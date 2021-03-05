#include <modules/rendering/renderer.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

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
#include <modules/rendering/model/rendertexture.h>
#include <modules/rendering/model/rendermaterial.h>
#include <modules/rendering/model/rendermodel.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::string;
		using std::unordered_map;
		using std::unordered_set;

		const unsigned int Renderer::ROOT_TAG_GROUP = 0;

		Renderer& Renderer::MakeRenderer(
			const string& windowTitle,
			unsigned int windowWidth,
			unsigned int windowHeight
		)
		{
			if (SDL_Init(SDL_INIT_VIDEO) < 0) {
				throw string("Failed to initialize SDL Video. ") + SDL_GetError();
			}

			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
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
				throw string("Unable to create SDL window. ") + SDL_GetError();
			}

			auto glContext = SDL_GL_CreateContext(pWindow);

			SDL_GL_SetSwapInterval(0); // unlimited powaaaa

			if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
				throw "Failed to initialize GLAD";
			}

			glViewport(0, 0, windowWidth, windowHeight);
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
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
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
			if (&dependency == &rootCompositingNode)
			{
				throw "Cannot attach root CompositingNode as a dependency";
			}
			if (_compositingNodes.find(&dependent) == _compositingNodes.end())
			{
				throw "Cannot attach unknown dependent CompositingNode";
			}
			if (_compositingNodes.find(&dependency) == _compositingNodes.end())
			{
				throw "Cannot attach unknown dependency CompositingNode";
			}
#endif

			dependent.AddDependency(dependency);
		}

		void Renderer::DetachCompositingNodeDependency(
			RenderCompositingNode& dependent,
			RenderCompositingNode& dependency
		)
		{
#ifdef __PRE_DEBUG__
			if (_compositingNodes.find(&dependent) == _compositingNodes.end())
			{
				throw "Cannot detach unknown dependent CompositingNode";
			}
			if (_compositingNodes.find(&dependency) == _compositingNodes.end())
			{
				throw "Cannot detach unknown dependency CompositingNode";
			}
#endif

			dependent.RemoveDependency(dependency);
		}

		void Renderer::DeallocateCompositingNode(RenderCompositingNode& compositingNode)
		{
			auto pCompositingNode = &compositingNode;
#ifdef __PRE_DEBUG__
			if (pCompositingNode == &rootCompositingNode)
			{
				throw "Cannot deallocate root CompositingNode";
			}
			if (_compositingNodes.find(pCompositingNode) == _compositingNodes.end())
			{
				throw "Cannot deallocate unknown CompositingNode";
			}
			if (_compositingNodeCameraPairs.find(pCompositingNode) != _compositingNodeCameraPairs.end())
			{
				throw "Cannot deallocate paired CompositingNode";
			}

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
			if (_cameras.find(pCamera) == _cameras.end())
			{
				throw "Cannot deallocate unknown Camera";
			}
			if (_cameraCompositingNodePairs.find(pCamera) != _cameraCompositingNodePairs.end())
			{
				throw "Cannot deallocate paired Camera";
			}

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
			if (_cameras.find(pCamera) == _cameras.end())
			{
				throw "Cannot bind unknown Camera";
			}
			if (&compositingNode != &rootCompositingNode && _compositingNodes.find(pCompositingNode) == _compositingNodes.end())
			{
				throw "Cannot bind unknown CompositingNode";
			}
			if (_cameraCompositingNodePairs.find(pCamera) != _cameraCompositingNodePairs.end())
			{
				throw "Camera is already bound to a CompositingNode";
			}
			if (_compositingNodeCameraPairs.find(pCompositingNode) != _compositingNodeCameraPairs.end())
			{
				throw "CompositingNode is already bound to a Camera";
			}
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
			if (_cameras.find(&camera) == _cameras.end())
			{
				throw "Cannot unbind unknown Camera";
			}
			if (&compositingNode != &rootCompositingNode && _compositingNodes.find(&compositingNode) == _compositingNodes.end())
			{
				throw "Cannot unbind unknown CompositingNode";
			}

			if (!(
				itCameraPair != _cameraCompositingNodePairs.end() &&
				itCameraPair->first == &camera &&
				itCompositingNodePair != _compositingNodeCameraPairs.end() &&
				itCompositingNodePair->first == &compositingNode
				))
			{
				throw "Cannot unbind unknown compositing pair";
			}

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
			if (_vertexShaders.find(pVertexShader) == _vertexShaders.end())
			{
				throw "Cannot deallocate unknown VertexShader";
			}

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
			if (_fragmentShaders.find(pFragmentShader) == _fragmentShaders.end())
			{
				throw "Cannot deallocate unknown FragmentShader";
			}

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
			if (_shaderPrograms.find(pShaderProgram) == _shaderPrograms.end())
			{
				throw "Cannot deallocate unknown ShaderProgram";
			}

			_shaderPrograms.erase(pShaderProgram);
#endif

			delete pShaderProgram;
		}

		RenderMesh& Renderer::AllocateMesh()
		{
			auto mesh = new RenderMesh();

#ifdef __PRE_DEBUG__
			_meshes.insert(mesh);
#endif

			return *mesh;
		}

		void Renderer::DeallocateMesh(RenderMesh& mesh)
		{
			auto pMesh = &mesh;

#ifdef __PRE_DEBUG__
			if (_meshes.find(pMesh) == _meshes.end())
			{
				throw "Cannot deallocate unknown Mesh";
			}

			_meshes.erase(_meshes.find(pMesh));
#endif

			delete pMesh;
		}

		RenderTexture& Renderer::AllocateTexture()
		{
			auto texture = new RenderTexture();

#ifdef __PRE_DEBUG__
			_textures.insert(texture);
#endif

			return *texture;
		}

		void Renderer::DeallocateTexture(RenderTexture& texture)
		{
			auto pTexture = &texture;

#ifdef __PRE_DEBUG__
			if (_textures.find(pTexture) == _textures.end())
			{
				throw "Cannot deallocate unknown Texture";
			}

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
			if (_materials.find(pMaterial) == _materials.end())
			{
				throw "Cannot deallocate unknown Material";
			}

			_materials.erase(pMaterial);
#endif

			delete pMaterial;
		}

		RenderModel& Renderer::AllocateModel()
		{
			auto pModel = new RenderModel();

#ifdef __PRE_DEBUG__
			_models.insert(pModel);
			_modelTagGroups[pModel] = std::move(unordered_set<unsigned int>());
#endif

			return *pModel;
		}

		void Renderer::SetModelMesh(RenderModel& model, RenderMesh* pMesh)
		{
#ifdef __PRE_DEBUG__
			if (_models.find(&model) == _models.end())
			{
				throw "Cannot operate on unknown Model";
			}
			if (_meshes.find(pMesh) == _meshes.end())
			{
				throw "Cannot set unknown Mesh to Model";
			}
#endif

			model.SetMesh(pMesh);
		}

		void Renderer::SetModelMaterial(RenderModel& model, RenderMaterial* pMaterial)
		{
#ifdef __PRE_DEBUG__
			if (_models.find(&model) == _models.end())
			{
				throw "Cannot operate on unknown Model";
			}
			if (_materials.find(pMaterial) == _materials.end())
			{
				throw "Cannot set unknown Material to Model";
			}
#endif

			model.SetMaterial(pMaterial);
		}

		void Renderer::DeallocateModel(RenderModel& model)
		{
			auto pModel = &model;

#ifdef __PRE_DEBUG__
			if (_models.find(pModel) == _models.end())
			{
				throw "Cannot deallocate unknown Model";
			}
			if (!_modelTagGroups.find(pModel)->second.empty())
			{
				throw "Cannot deallocate Model still in tag group(s)";
			}

			_models.erase(pModel);
#endif

			delete pModel;
		}

		void Renderer::DeclareTagGroup(unsigned int tagGroup)
		{
#ifdef __PRE_DEBUG__
			if (_tagGroups.find(tagGroup) != _tagGroups.end())
			{
				throw "Tag group already exists";
			}
#endif
			_tagGroups[tagGroup] = std::move(unordered_set<RenderModel*>());
		}

		void Renderer::AddModelToTagGroup(RenderModel& model, unsigned int tagGroup)
		{
#ifdef __PRE_DEBUG__
			if (_models.find(&model) == _models.end())
			{
				throw "Cannot add unknown Model to tag group";
			}
			if (_tagGroups.find(tagGroup) == _tagGroups.end())
			{
				throw "Cannot add Model to undeclared tag group";
			}
			auto& modelTagGroup = _modelTagGroups.find(&model)->second;
			if (modelTagGroup.find(tagGroup) != modelTagGroup.end())
			{
				throw "Model already belongs to tag group";
			}
			modelTagGroup.insert(tagGroup);
#endif
			_tagGroups[tagGroup].insert(&model);
		}

		void Renderer::RemoveModelFromTagGroup(RenderModel& model, unsigned int tagGroup)
		{
#ifdef __PRE_DEBUG__
			if (_models.find(&model) == _models.end())
			{
				throw "Cannot remove unknown Model from tag group";
			}
			if (_tagGroups.find(tagGroup) == _tagGroups.end())
			{
				throw "Cannot remove Model from undeclared tag group";
			}
			auto& modelTagGroup = _modelTagGroups.find(&model)->second;
			if (modelTagGroup.find(tagGroup) == modelTagGroup.end())
			{
				throw "Model does not belong to tag group";
			}
			modelTagGroup.erase(tagGroup);
#endif
			_tagGroups[tagGroup].erase(&model);
		}

		void Renderer::RevokeTagGroup(unsigned int tagGroup)
		{
#ifdef __PRE_DEBUG__
			if (tagGroup == ROOT_TAG_GROUP)
			{
				throw "Cannot revoke root tag group";
			}
			if (_tagGroups.find(tagGroup) == _tagGroups.end())
			{
				throw "Cannot revoke unknown tag group";
			}
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
			_tagGroups[ROOT_TAG_GROUP] = std::move(unordered_set<RenderModel*>());
		}

		Renderer::~Renderer()
		{
#ifdef __PRE_DEBUG__
			delete& rootCompositingNode;
			for (auto it = _compositingNodes.begin(); it != _compositingNodes.end(); ++it)
			{
				delete *it;
			}
			for (auto it = _cameras.begin(); it != _cameras.end(); ++it)
			{
				delete *it;
			}
			for (auto it = _vertexShaders.begin(); it != _vertexShaders.end(); ++it)
			{
				delete *it;
			}
			for (auto it = _fragmentShaders.begin(); it != _fragmentShaders.end(); ++it)
			{
				delete *it;
			}
			for (auto it = _shaderPrograms.begin(); it != _shaderPrograms.end(); ++it)
			{
				delete *it;
			}
			for (auto it = _meshes.begin(); it != _meshes.end(); ++it)
			{
				delete *it;
			}
			for (auto it = _textures.begin(); it != _textures.end(); ++it)
			{
				delete *it;
			}
			for (auto it = _materials.begin(); it != _materials.end(); ++it)
			{
				delete *it;
			}
			for (auto it = _models.begin(); it != _models.end(); ++it)
			{
				delete *it;
			}
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
			if (visited.find(pCurrentNode) != visited.end()) {

				throw "Cyclic compositing dependency detected";
			}

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
			if (_tagGroups.find(currentNode._tagGroup) == _tagGroups.end())
			{
				throw "Cannot composite using undeclared tag group";
			}
#endif

			const glm::mat4* pViewMatrix = &MAT4_IDENTITY;

			auto itCamera = _compositingNodeCameraPairs.find(pCurrentNode);
			if (itCamera != _compositingNodeCameraPairs.end())
			{
				pViewMatrix = &itCamera->second->GetViewProjectionMatrix();
			}
			auto& viewMatrix = *pViewMatrix;

			RenderCompositingTarget::Bind(currentNode._pCompositingTarget);
			auto& tagModelSet = _tagGroups[currentNode._tagGroup];
			for (auto it = tagModelSet.begin(); it != tagModelSet.end(); ++it)
			{
				auto& model = **it;
				model.Render(viewMatrix);
			}
			RenderCompositingTarget::Bind(nullptr);

#ifdef __PRE_DEBUG__
			visited.erase(pCurrentNode);
#endif
			rendered.insert(pCurrentNode);
		}
	} // namespace RenderingModule
} // namespace PRE