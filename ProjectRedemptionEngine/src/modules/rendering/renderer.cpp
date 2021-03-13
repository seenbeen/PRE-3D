#include <modules/rendering/renderer.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <SDL.h>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <modules/rendering/compositing/rendercompositingtarget.h>
#include <modules/rendering/compositing/rendercompositingnode.h>
#include <modules/rendering/rendercamera.h>
#include <modules/rendering/shader/rendervertexshader.h>
#include <modules/rendering/shader/renderfragmentshader.h>
#include <modules/rendering/shader/rendershaderprogram.h>
#include <modules/rendering/model/rendermesh.h>
#include <modules/rendering/model/renderskeleton.h>
#include <modules/rendering/model/rendermaterial.h>
#include <modules/rendering/model/rendermodel.h>
#include <modules/rendering/texture/rendertexture.h>

#ifdef __PRE_DEBUG__
#include <assert.h>
#endif

namespace PRE
{
	namespace RenderingModule
	{
		using std::string;
		using std::unordered_map;
		using std::unordered_set;
		using std::vector;

		const unsigned int Renderer::ROOT_TAG_GROUP = 0;

		Renderer& Renderer::MakeRenderer(
			const string& windowTitle,
			unsigned int windowWidth,
			unsigned int windowHeight
		)
		{
			if (SDL_Init(SDL_INIT_VIDEO) < 0) {
				throw std::exception((string("Failed to initialize SDL Video. ") + SDL_GetError()).c_str());
			}

			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			auto pWindow = SDL_CreateWindow(
				windowTitle.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				windowWidth,
				windowHeight,
				SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
			);

			if (pWindow == nullptr)
			{
				throw std::exception((string("Unable to create SDL window. ") + SDL_GetError()).c_str());
			}

			auto glContext = SDL_GL_CreateContext(pWindow);

			SDL_GL_SetSwapInterval(0); // unlimited powaaaa

			if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
				throw std::exception("Failed to initialize GLAD");
			}

			glViewport(0, 0, windowWidth, windowHeight);

			glEnable(GL_DEPTH_TEST);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			return *(new Renderer(*pWindow, glContext));
		}

		void Renderer::SetActiveRenderer(Renderer& renderer)
		{
			// Note: idk why this seems to fudge up the display when called
			//SDL_GL_MakeCurrent(&renderer._window, renderer._glContext);
		}

		void Renderer::ShutdownRenderer(Renderer& renderer)
		{
			SDL_GL_DeleteContext(renderer._glContext);
			SDL_DestroyWindow(&renderer._window);
			delete& renderer;
		}

		// TODO: spatial query optimization
		void Renderer::Update()
		{
			glDepthMask(GL_TRUE);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			unordered_set<RenderCompositingNode*> visited;
			unordered_set<RenderCompositingNode*> rendered;

#if __PRE_DEBUG__
			UpdateRecurse(rootCompositingNode, rendered, visited);
#else
			UpdateRecurse(rootCompositingNode, rendered);
#endif

			SDL_GL_SwapWindow(&_window);
		}

		RenderCompositingNode& Renderer::AllocateCompositingNode(
			unsigned int renderTag,
			unsigned int width,
			unsigned int height
		)
		{
			auto pCompositingTarget = new RenderCompositingTarget(width, height);
			auto pCompositingNode = new RenderCompositingNode(renderTag, pCompositingTarget);

#ifdef __PRE_DEBUG__
			_compositingNodes.insert(pCompositingNode);
#endif

			return *pCompositingNode;
		}

		void Renderer::AttachCompositingNodeDependency(
			RenderCompositingNode& dependent,
			RenderCompositingNode& dependency
		)
		{
#ifdef __PRE_DEBUG__
			assert(&dependency != &rootCompositingNode);
			assert(_compositingNodes.find(&dependent) != _compositingNodes.end());
			assert(_compositingNodes.find(&dependency) != _compositingNodes.end());
#endif

			dependent.AddDependency(dependency);
		}

		void Renderer::DetachCompositingNodeDependency(
			RenderCompositingNode& dependent,
			RenderCompositingNode& dependency
		)
		{
#ifdef __PRE_DEBUG__
			assert(_compositingNodes.find(&dependent) != _compositingNodes.end());
			assert(_compositingNodes.find(&dependency) != _compositingNodes.end());
#endif

			dependent.RemoveDependency(dependency);
		}

		void Renderer::DeallocateCompositingNode(RenderCompositingNode& compositingNode)
		{
			auto pCompositingNode = &compositingNode;

#ifdef __PRE_DEBUG__
			assert(pCompositingNode != &rootCompositingNode);
			assert(_compositingNodes.find(pCompositingNode) != _compositingNodes.end());
			assert(_compositingNodeCameraPairs.find(pCompositingNode) == _compositingNodeCameraPairs.end());
			_compositingNodes.erase(pCompositingNode);
#endif

			delete pCompositingNode->_pCompositingTarget;
			delete pCompositingNode;
		}

		RenderCamera& Renderer::AllocateCamera(
			const CameraKind& kind,
			float size,
			float aspectRatio,
			float nearClippingPlane,
			float farClippingPlane
		)
		{
			auto camera = new RenderCamera(
				kind == CameraKind::ORTHOGRAPHIC ?
				RenderCamera::Kind::ORTHOGRAPHIC :
				RenderCamera::Kind::PERSPECTIVE,
				size,
				aspectRatio,
				nearClippingPlane,
				farClippingPlane
			);

#ifdef __PRE_DEBUG__
			_cameras.insert(camera);
#endif

			return *camera;
		}

		void Renderer::DeallocateCamera(RenderCamera& camera)
		{
			auto pCamera = &camera;

#ifdef __PRE_DEBUG__
			assert(_cameras.find(pCamera) != _cameras.end());
			assert(_cameraCompositingNodePairs.find(pCamera) == _cameraCompositingNodePairs.end());
			_cameras.erase(pCamera);
#endif

			delete pCamera;
		}

		void Renderer::BindCompositingPair(
			RenderCamera& camera,
			RenderCompositingNode& compositingNode
		)
		{
			auto pCamera = &camera;
			auto pCompositingNode = &compositingNode;

#ifdef __PRE_DEBUG__
			assert(_cameras.find(pCamera) != _cameras.end());
			assert(!(
				&compositingNode != &rootCompositingNode &&
				_compositingNodes.find(pCompositingNode) == _compositingNodes.end())
			);
			assert(_cameraCompositingNodePairs.find(pCamera) == _cameraCompositingNodePairs.end());
			assert(_compositingNodeCameraPairs.find(pCompositingNode) == _compositingNodeCameraPairs.end());
#endif

			_compositingNodeCameraPairs[pCompositingNode] = pCamera;

#ifdef __PRE_DEBUG__
			_cameraCompositingNodePairs[pCamera] = pCompositingNode;
#endif
		}

		void Renderer::UnbindCompositingPair(
			RenderCamera& camera,
			RenderCompositingNode& compositingNode
		)
		{
#ifdef __PRE_DEBUG__
			auto itCameraPair = _cameraCompositingNodePairs.find(&camera);
#endif

			auto itCompositingNodePair = _compositingNodeCameraPairs.find(&compositingNode);

#ifdef __PRE_DEBUG__
			assert(_cameras.find(&camera) != _cameras.end());
			assert(!(
				&compositingNode != &rootCompositingNode &&
				_compositingNodes.find(&compositingNode) == _compositingNodes.end()
			));
			assert(
				itCameraPair != _cameraCompositingNodePairs.end() &&
				itCameraPair->first == &camera &&
				itCompositingNodePair != _compositingNodeCameraPairs.end() &&
				itCompositingNodePair->first == &compositingNode
			);
			_cameraCompositingNodePairs.erase(itCameraPair);
#endif

			_compositingNodeCameraPairs.erase(itCompositingNodePair);
		}

		RenderVertexShader& Renderer::AllocateVertexShader(const string& shaderSource)
		{
			auto vertexShader = new RenderVertexShader(shaderSource);

#ifdef __PRE_DEBUG__
			_vertexShaders.insert(vertexShader);
#endif

			return *vertexShader;
		}

		void Renderer::DeallocateVertexShader(const RenderVertexShader& vertexShader)
		{
			auto pVertexShader = &vertexShader;

#ifdef __PRE_DEBUG__
			assert(_vertexShaders.find(pVertexShader) != _vertexShaders.end());
			_vertexShaders.erase(pVertexShader);
#endif

			delete pVertexShader;
		}

		RenderFragmentShader& Renderer::AllocateFragmentShader(const string& shaderSource)
		{
			auto fragmentShader = new RenderFragmentShader(shaderSource);

#ifdef __PRE_DEBUG__
			_fragmentShaders.insert(fragmentShader);
#endif

			return *fragmentShader;
		}

		void Renderer::DeallocateFragmentShader(const RenderFragmentShader& fragmentShader)
		{
			auto pFragmentShader = &fragmentShader;

#ifdef __PRE_DEBUG__
			assert(_fragmentShaders.find(pFragmentShader) != _fragmentShaders.end());
			_fragmentShaders.erase(pFragmentShader);
#endif

			delete pFragmentShader;
		}

		RenderShaderProgram& Renderer::AllocateShaderProgram(
			const RenderVertexShader& vertexShader,
			const RenderFragmentShader& fragmentShader
		)
		{
			auto shaderProgram = new RenderShaderProgram(vertexShader, fragmentShader);

#ifdef __PRE_DEBUG__
			_shaderPrograms.insert(shaderProgram);
#endif

			return *shaderProgram;
		}

		void Renderer::DeallocateShaderProgram(const RenderShaderProgram& shaderProgram)
		{
			auto pShaderProgram = &shaderProgram;

#ifdef __PRE_DEBUG__
			assert(_shaderPrograms.find(pShaderProgram) != _shaderPrograms.end());
			_shaderPrograms.erase(pShaderProgram);
#endif

			delete pShaderProgram;
		}

		RenderMesh& Renderer::AllocateMesh(
			unsigned int nVertices,
			const glm::vec3* vertices,
			const glm::vec3* normals,
			const glm::vec3* tangents,
			const glm::vec3* biTangents,
			const glm::vec2* uvs,
			const glm::ivec4* vertexBoneIds,
			const glm::vec4* vertexBoneWeights,
			unsigned int nTriangleElements,
			const unsigned int* const triangleElements
		)
		{
			auto mesh = new RenderMesh(
				nVertices,
				vertices,
				normals,
				tangents,
				biTangents,
				uvs,
				vertexBoneIds,
				vertexBoneWeights,
				nTriangleElements,
				triangleElements
			);

#ifdef __PRE_DEBUG__
			_meshes.insert(mesh);
#endif

			return *mesh;
		}

		void Renderer::DeallocateMesh(RenderMesh& mesh)
		{
			auto pMesh = &mesh;

#ifdef __PRE_DEBUG__
			assert(_meshes.find(pMesh) != _meshes.end());
			_meshes.erase(pMesh);
#endif

			delete pMesh;
		}

		RenderSkeleton& Renderer::AllocateSkeleton(
			const RenderSkeleton::BoneConfig& rootBone
		)
		{
			auto pSkeleton = new RenderSkeleton(rootBone);

#ifdef __PRE_DEBUG__
			_skeletons.insert(pSkeleton);
#endif

			return *pSkeleton;
		}

		void Renderer::DeallocateSkeleton(RenderSkeleton& skeleton)
		{
			auto pSkeleton = &skeleton;

#ifdef __PRE_DEBUG__
			assert(_skeletons.find(pSkeleton) != _skeletons.end());
			_skeletons.erase(pSkeleton);
#endif

			delete pSkeleton;
		}

		RenderTexture& Renderer::AllocateTexture(
			unsigned int width,
			unsigned int height,
			const unsigned char* data
		)
		{
			auto texture = new RenderTexture(
				width,
				height,
				data
			);

#ifdef __PRE_DEBUG__
			_textures.insert(texture);
#endif

			return *texture;
		}

		RenderTexture& Renderer::AllocateTexture(
			unsigned int rightWidth,
			unsigned int rightHeight,
			const unsigned char* rightData,
			unsigned int leftWidth,
			unsigned int leftHeight,
			const unsigned char* leftData,
			unsigned int topWidth,
			unsigned int topHeight,
			const unsigned char* topData,
			unsigned int bottomWidth,
			unsigned int bottomHeight,
			const unsigned char* bottomData,
			unsigned int frontWidth,
			unsigned int frontHeight,
			const unsigned char* frontData,
			unsigned int backWidth,
			unsigned int backHeight,
			const unsigned char* backData
		)
		{
			auto texture = new RenderTexture(
				rightWidth, rightHeight, rightData,
				leftWidth, leftHeight, leftData,
				topWidth, topHeight, topData,
				bottomWidth, bottomHeight, bottomData,
				frontWidth, frontHeight, frontData,
				backWidth, backHeight, backData
			);

#ifdef __PRE_DEBUG__
			_textures.insert(texture);
#endif

			return *texture;
		}

		void Renderer::DeallocateTexture(RenderTexture& texture)
		{
			auto pTexture = &texture;

#ifdef __PRE_DEBUG__
			assert(_textures.find(pTexture) != _textures.end());
			_textures.erase(pTexture);
#endif

			delete pTexture;
		}

		RenderMaterial& Renderer::AllocateMaterial()
		{
			auto material = new RenderMaterial();

#ifdef __PRE_DEBUG__
			_materials.insert(material);
#endif

			return *material;
		}

		void Renderer::DeallocateMaterial(RenderMaterial& material)
		{
			auto pMaterial = &material;

#ifdef __PRE_DEBUG__
			assert(_materials.find(pMaterial) != _materials.end());
			_materials.erase(pMaterial);
#endif

			delete pMaterial;
		}

		RenderModel& Renderer::AllocateModel()
		{
			auto pModel = new RenderModel();

#ifdef __PRE_DEBUG__
			_models.insert(pModel);
			_modelTagGroups[pModel];
#endif

			return *pModel;
		}

		void Renderer::SetModelMesh(RenderModel& model, RenderMesh* pMesh)
		{
#ifdef __PRE_DEBUG__
			assert(_models.find(&model) != _models.end());
			assert(!(
				pMesh != nullptr &&
				_meshes.find(pMesh) == _meshes.end()
			));
#endif

			model.SetMesh(pMesh);
		}

		void Renderer::SetModelSkeleton(
			RenderModel& model,
			RenderSkeleton* pSkeleton
		)
		{
#ifdef __PRE_DEBUG__
			assert(_models.find(&model) != _models.end());
			assert(!(
				pSkeleton != nullptr &&
				_skeletons.find(pSkeleton) == _skeletons.end()
			));
#endif

			model.SetSkeleton(pSkeleton);
		}

		void Renderer::SetModelMaterial(RenderModel& model, RenderMaterial* pMaterial)
		{
#ifdef __PRE_DEBUG__
			assert(_models.find(&model) != _models.end());
			assert(!(
				pMaterial != nullptr &&
				_materials.find(pMaterial) == _materials.end()
			));
#endif

			model.SetMaterial(pMaterial);
		}

		void Renderer::DeallocateModel(RenderModel& model)
		{
			auto pModel = &model;

#ifdef __PRE_DEBUG__
			assert(_models.find(pModel) != _models.end());
			assert(_modelTagGroups.find(pModel)->second.empty());
			_models.erase(pModel);
			_modelTagGroups.erase(pModel);
#endif

			delete pModel;
		}

		void Renderer::DeclareTagGroup(unsigned int tagGroup)
		{
#ifdef __PRE_DEBUG__
			assert(_tagGroups.find(tagGroup) == _tagGroups.end());
#endif
			_tagGroups[tagGroup];
		}

		void Renderer::AddModelToTagGroup(RenderModel& model, unsigned int tagGroup)
		{
#ifdef __PRE_DEBUG__
			assert(_models.find(&model) != _models.end());
			assert(_tagGroups.find(tagGroup) != _tagGroups.end());
			auto& modelTagGroup = _modelTagGroups.find(&model)->second;
			assert(modelTagGroup.find(tagGroup) == modelTagGroup.end());
			modelTagGroup.insert(tagGroup);
#endif

			_tagGroups[tagGroup].insert(&model);
		}

		void Renderer::RemoveModelFromTagGroup(RenderModel& model, unsigned int tagGroup)
		{
#ifdef __PRE_DEBUG__
			assert(_models.find(&model) != _models.end());
			assert(_tagGroups.find(tagGroup) != _tagGroups.end());
			auto& modelTagGroup = _modelTagGroups.find(&model)->second;
			assert(modelTagGroup.find(tagGroup) != modelTagGroup.end());
			modelTagGroup.erase(tagGroup);
#endif

			_tagGroups[tagGroup].erase(&model);
		}

		void Renderer::RevokeTagGroup(unsigned int tagGroup)
		{
#ifdef __PRE_DEBUG__
			assert(tagGroup != ROOT_TAG_GROUP);
			assert(_tagGroups.find(tagGroup) != _tagGroups.end());
#endif

			_tagGroups.erase(tagGroup);
		}

		const glm::mat4 Renderer::MAT4_IDENTITY = glm::mat4();

		Renderer::Renderer(SDL_Window& window, SDL_GLContext& glContext)
			:
			rootCompositingNode(*(new RenderCompositingNode(ROOT_TAG_GROUP, nullptr))),
			_window(window),
			_glContext(glContext)
		{
			_tagGroups[ROOT_TAG_GROUP];
		}

		Renderer::~Renderer()
		{
			delete& rootCompositingNode;
#ifdef __PRE_DEBUG__
			assert(_compositingNodes.size() == 0);
			assert(_cameras.size() == 0);
			assert(_compositingNodeCameraPairs.size() == 0);
			assert(_cameraCompositingNodePairs.size() == 0);
			assert(_vertexShaders.size() == 0);
			assert(_fragmentShaders.size() == 0);
			assert(_shaderPrograms.size() == 0);
			assert(_meshes.size() == 0);
			assert(_skeletons.size() == 0);
			assert(_textures.size() == 0);
			assert(_materials.size() == 0);
			assert(_models.size() == 0);
			assert(_tagGroups.size() == 1);
			assert(_modelTagGroups.size() == 0);
#endif
		}

		void Renderer::UpdateRecurse(
#ifdef __PRE_DEBUG__
			RenderCompositingNode& currentNode,
			unordered_set<RenderCompositingNode*>& rendered,
			unordered_set<RenderCompositingNode*>& visited
#else
			RenderCompositingNode& currentNode,
			unordered_set<RenderCompositingNode*>& rendered
#endif
		)
		{
			auto pCurrentNode = &currentNode;
			if (rendered.find(pCurrentNode) != rendered.end())
			{
				return;
			}

#ifdef __PRE_DEBUG__
			assert(visited.find(pCurrentNode) == visited.end());
			visited.insert(pCurrentNode);
#endif

			auto& dependencies = currentNode.GetDependencies();
			for (auto it = dependencies.begin(); it != dependencies.end(); ++it)
			{

#ifdef __PRE_DEBUG__
				UpdateRecurse(**it, rendered, visited);
#else
				UpdateRecurse(**it, rendered);
#endif

			}

#ifdef __PRE_DEBUG__
			assert(_tagGroups.find(currentNode._tagGroup) != _tagGroups.end());
#endif

			auto itCamera = _compositingNodeCameraPairs.find(pCurrentNode);
			if (itCamera != _compositingNodeCameraPairs.end())
			{
				auto& camera = *itCamera->second;
				const auto& viewMatrix = camera.GetViewMatrix();
				const auto& projectionMatrix = camera.GetProjectionMatrix();

				RenderCompositingTarget::Bind(currentNode._pCompositingTarget);
				auto& tagModelSet = _tagGroups[currentNode._tagGroup];
				for (auto it = tagModelSet.begin(); it != tagModelSet.end(); ++it)
				{
					auto& model = **it;
					model.Render(viewMatrix, projectionMatrix);
				}
				RenderCompositingTarget::Bind(nullptr);
			}

#ifdef __PRE_DEBUG__
			visited.erase(pCurrentNode);
#endif
			rendered.insert(pCurrentNode);
		}
	} // namespace RenderingModule
} // namespace PRE