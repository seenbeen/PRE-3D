#include <modules/rendering/renderer.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <SDL.h>

#include <glm/glm.hpp>

#include <glad/glad.h>

#include <modules/rendering/compositing/compositingtarget.h>
#include <modules/rendering/compositing/compositingnode.h>

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
			delete &renderer;
		}

		// TODO: spatial query optimization
		void Renderer::Update()
		{
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			unordered_set<CompositingNode*> visited;
			for (auto it = _rootCompositingNodes.begin(); it != _rootCompositingNodes.end(); ++it)
			{
				auto pCurrentNode = *it;

#ifdef __PRE_DEBUG__
				UpdateRecurse(*pCurrentNode, visited);
#else
				UpdateRecurse(*pCurrentNode);
#endif

				auto itTag = _models.find(pCurrentNode->_renderTag);
				if (itTag == _models.end())
				{
					continue;
				}

				const glm::mat4* pViewMatrix = &MAT4_IDENTITY;

				auto itCamera = _compositingNodeCameraPairs.find(pCurrentNode);
				if (itCamera != _compositingNodeCameraPairs.end())
				{
					pViewMatrix = &itCamera->second->GetViewProjectionMatrix();
				}
				auto& viewMatrix = *pViewMatrix;

				CompositingTarget::Bind(nullptr);
				auto& tagModelSet = itTag->second;
				for (auto it = tagModelSet.begin(); it != tagModelSet.end(); ++it)
				{
					auto& model = **it;
					model.Render(viewMatrix);
				}
				CompositingTarget::Bind(nullptr);
			}

			SDL_GL_SwapWindow(&_window);
		}

		CompositingNode& Renderer::AllocateCompositingNode(
			unsigned int renderTag,
			unsigned int width,
			unsigned int height
		)
		{
			auto pCompositingTarget = new CompositingTarget(width, height);
			auto pCompositingNode = new CompositingNode(renderTag, *pCompositingTarget);
			_compositingNodes.insert(pCompositingNode);
			return *pCompositingNode;
		}

		void Renderer::AttachRootCompositingNodeDependency(CompositingNode& dependency)
		{
			auto pDependency = &dependency;

#ifdef __PRE_DEBUG__
			if (_compositingNodes.find(pDependency) == _compositingNodes.end())
			{
				throw "Cannot attach unknown dependency CompositingNode";
			}
			if (_rootCompositingNodes.find(pDependency) != _rootCompositingNodes.end())
			{
				throw "duplicate CompositingNode dependency on root detected";
			}
#endif

			_rootCompositingNodes.insert(pDependency);
		}

		void Renderer::DetachRootCompositingNodeDependency(CompositingNode& dependency)
		{
			auto pDependency = &dependency;

#ifdef __PRE_DEBUG__
			if (_compositingNodes.find(pDependency) == _compositingNodes.end())
			{
				throw "Cannot detach unknown dependency CompositingNode";
			}
			if (_rootCompositingNodes.find(pDependency) == _rootCompositingNodes.end())
			{
				throw "cannot remove unknown CompositingNode dependency from root";
			}
#endif

			_rootCompositingNodes.erase(pDependency);
		}

		void Renderer::AttachCompositingNodeDependency(
			CompositingNode& dependent,
			CompositingNode& dependency
		)
		{
#ifdef __PRE_DEBUG__
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
			CompositingNode& dependent,
			CompositingNode& dependency
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

		void Renderer::DeallocateCompositingNode(CompositingNode& compositingNode)
		{
			auto pCompositingNode = &compositingNode;
#ifdef __PRE_DEBUG__
			if (_compositingNodes.find(pCompositingNode) == _compositingNodes.end())
			{
				throw "Cannot deallocate unknown CompositingNode";
			}
			if (_compositingNodeCameraPairs.find(pCompositingNode) != _compositingNodeCameraPairs.end())
			{
				throw "Cannot deallocate paired CompositingNode";
			}
#endif

			_compositingNodes.erase(pCompositingNode);
			delete &pCompositingNode->_compositingTarget;
			delete pCompositingNode;
		}

		RenderCamera& Renderer::AllocateCamera(
			const RenderCamera::Kind& kind,
			float size,
			float aspectRatio,
			float nearClippingPlane,
			float farClippingPlane
		)
		{
			auto camera = new RenderCamera(
				kind,
				size,
				aspectRatio,
				nearClippingPlane,
				farClippingPlane
			);
			_cameras.insert(camera);
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
#endif

			_cameras.erase(pCamera);
			delete pCamera;
		}

		void Renderer::BindCompositingPair(
			RenderCamera& camera,
			CompositingNode& compositingNode
		)
		{
			auto pCamera = &camera;
			auto pCompositingNode = &compositingNode;
#ifdef __PRE_DEBUG__
			if (_cameras.find(pCamera) == _cameras.end())
			{
				throw "Cannot bind unknown Camera";
			}
			if (_compositingNodes.find(pCompositingNode) == _compositingNodes.end())
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
			_cameraCompositingNodePairs[pCamera] = pCompositingNode;
		}

		void Renderer::UnbindCompositingPair(
			RenderCamera& camera,
			CompositingNode& compositingNode
		)
		{
			auto itCameraPair = _cameraCompositingNodePairs.find(&camera);
			auto itCompositingNodePair = _compositingNodeCameraPairs.find(&compositingNode);

#ifdef __PRE_DEBUG__
			if (_cameras.find(&camera) == _cameras.end())
			{
				throw "Cannot unbind unknown Camera";
			}
			if (_compositingNodes.find(&compositingNode) == _compositingNodes.end())
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
#endif

			_cameraCompositingNodePairs.erase(itCameraPair);
			_compositingNodeCameraPairs.erase(itCompositingNodePair);
		}

		const RenderVertexShader& Renderer::AllocateVertexShader(const string& shaderSource)
		{
			auto vertexShader = new RenderVertexShader(shaderSource);
			_vertexShaders.insert(vertexShader);
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
#endif

			_vertexShaders.erase(pVertexShader);
			delete pVertexShader;
		}

		const RenderFragmentShader& Renderer::AllocateFragmentShader(const string& shaderSource)
		{
			auto fragmentShader = new RenderFragmentShader(shaderSource);
			_fragmentShaders.insert(fragmentShader);
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
#endif

			_fragmentShaders.erase(pFragmentShader);
			delete pFragmentShader;
		}

		const RenderShaderProgram& Renderer::AllocateShaderProgram(
			const RenderVertexShader& vertexShader,
			const RenderFragmentShader& fragmentShader
		)
		{
			auto shaderProgram = new RenderShaderProgram(vertexShader, fragmentShader);
			_shaderPrograms.insert(shaderProgram);
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
#endif

			_shaderPrograms.erase(pShaderProgram);
			delete pShaderProgram;
		}

		RenderMesh& Renderer::AllocateMesh()
		{
			auto mesh = new RenderMesh();
			_meshes.insert(mesh);
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
#endif

			_meshes.erase(_meshes.find(pMesh));
			delete pMesh;
		}

		RenderTexture& Renderer::AllocateTexture()
		{
			auto texture = new RenderTexture();
			_textures.insert(texture);
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
#endif

			_textures.erase(pTexture);
			delete pTexture;
		}

		RenderMaterial& Renderer::AllocateMaterial(RenderShaderProgram& shaderProgram)
		{
			auto material = new RenderMaterial(shaderProgram);
			_materials.insert(material);
			return *material;
		}

		void Renderer::SetMaterialShader(RenderMaterial& material, RenderShaderProgram& shaderProgram)
		{
			auto pShaderProgram = &shaderProgram;

#ifdef __PRE_DEBUG__
			if (_materials.find(&material) == _materials.end())
			{
				throw "Cannot operate on unknown Material";
			}
			if (_shaderPrograms.find(pShaderProgram) == _shaderPrograms.end())
			{
				throw "Cannot set unknown ShaderProgram to Material";
			}
#endif

			material.SetShaderProgram(shaderProgram);
		}

		void Renderer::SetMaterialTextureBinding(
			RenderMaterial& material,
			RenderTexture* pTexture,
			GLenum bindUnit
		)
		{
#ifdef __PRE_DEBUG__
			if (_materials.find(&material) == _materials.end())
			{
				throw "Cannot operate on unknown Material";
			}
			if (pTexture != nullptr && _textures.find(pTexture) == _textures.end())
			{
				throw "Cannot bind unknown Texture to Material";
			}
#endif

			material.SetTextureBinding(pTexture, bindUnit);
		}

		void Renderer::SetMaterialTextureBinding(
			RenderMaterial& material,
			CompositingNode& compositingNode,
			CompositingNode::CompositingAttachment attachment,
			GLenum bindUnit
		)
		{
#ifdef __PRE_DEBUG__
			if (_materials.find(&material) == _materials.end())
			{
				throw "Cannot operate on unknown Material";
			}
			if (_compositingNodes.find(&compositingNode) == _compositingNodes.end())
			{
				throw "Cannot bind unknown CompositingNode to Material";
			}
#endif

			switch (attachment)
			{
				case CompositingNode::CompositingAttachment::POSITION:
					material.SetTextureBinding(
						&compositingNode._compositingTarget.GetPosition(),
						bindUnit
					);
				case CompositingNode::CompositingAttachment::NORMALS:
					material.SetTextureBinding(
						&compositingNode._compositingTarget.GetNormals(),
						bindUnit
					);
				case CompositingNode::CompositingAttachment::ALBEDO_SPECULAR:
					material.SetTextureBinding(
						&compositingNode._compositingTarget.GetAlbedoSpecular(),
						bindUnit
					);
			}
		}

		void Renderer::DeallocateMaterial(RenderMaterial& material)
		{
			auto pMaterial = &material;

#ifdef __PRE_DEBUG__
			if (_materials.find(pMaterial) == _materials.end())
			{
				throw "Cannot deallocate unknown Material";
			}
#endif

			_materials.erase(pMaterial);
			delete pMaterial;
		}

		RenderModel& Renderer::AllocateModel(unsigned int renderTag, RenderMesh& mesh, RenderMaterial& material)
		{
#ifdef __PRE_DEBUG__
			if (_meshes.find(&mesh) == _meshes.end())
			{
				throw "Cannot allocate Model with unknown Mesh";
			}
			if (_materials.find(&material) == _materials.end())
			{
				throw "Cannot allocate Model with unknown Material";
			}
#endif

			auto model = new RenderModel(renderTag, mesh, material);
			auto itTag = _models.find(renderTag);

			if (itTag == _models.end())
			{
				itTag = _models.insert(
					std::make_pair(
						renderTag,
						unordered_set<RenderModel*>()
					)
				).first;
			}

			auto& tagSet = itTag->second;
			tagSet.insert(model);
			return *model;
		}

		void Renderer::SetModelMesh(RenderModel& model, RenderMesh& mesh)
		{
#ifdef __PRE_DEBUG__
			auto itTag = _models.find(model._renderTag);
			if (itTag == _models.end())
			{
				throw "Cannot operate on unknown Model";
			}
			auto& tagSet = itTag->second;
			if (tagSet.find(&model) == tagSet.end())
			{
				throw "Cannot operate on unknown Model";
			}
			if (_meshes.find(&mesh) == _meshes.end())
			{
				throw "Cannot set unknown Mesh to Model";
			}
#endif

			model.SetMesh(mesh);
		}

		void Renderer::SetModelMaterial(RenderModel& model, RenderMaterial& material)
		{
#ifdef __PRE_DEBUG__
			auto itTag = _models.find(model._renderTag);
			if (itTag == _models.end())
			{
				throw "Cannot operate on unknown Model";
			}
			auto& tagSet = itTag->second;
			if (tagSet.find(&model) == tagSet.end())
			{
				throw "Cannot operate on unknown Model";
			}
			if (_materials.find(&material) == _materials.end())
			{
				throw "Cannot set unknown Material to Model";
			}
#endif

			model.SetMaterial(material);
		}

		void Renderer::DeallocateModel(RenderModel& model)
		{
			auto pModel = &model;
			auto itTag = _models.find(pModel->_renderTag);

#ifdef __PRE_DEBUG__
			if (itTag == _models.end())
			{
				throw "Cannot deallocate unknown Model";
			}
#endif

			auto& tagModelSet = itTag->second;

#ifdef __PRE_DEBUG__
			if (tagModelSet.find(pModel) == tagModelSet.end())
			{
				throw "Cannot deallocate unknown Model";
			}
#endif

			tagModelSet.erase(pModel);
			delete pModel;
		}

		const glm::mat4 Renderer::MAT4_IDENTITY = glm::mat4();

		Renderer::Renderer(SDL_Window& window, SDL_GLContext& glContext)
			:
			_window(window),
			_glContext(glContext) {}

		Renderer::~Renderer()
		{
			for (auto it = _vertexShaders.begin(); it != _vertexShaders.end(); ++it)
			{
				delete* it;
			}
			for (auto it = _fragmentShaders.begin(); it != _fragmentShaders.end(); ++it)
			{
				delete* it;
			}
			for (auto it = _shaderPrograms.begin(); it != _shaderPrograms.end(); ++it)
			{
				delete* it;
			}
			for (auto it = _meshes.begin(); it != _meshes.end(); ++it)
			{
				delete* it;
			}
			for (auto it = _textures.begin(); it != _textures.end(); ++it)
			{
				delete* it;
			}
			for (auto it = _materials.begin(); it != _materials.end(); ++it)
			{
				delete* it;
			}
			for (auto it = _models.begin(); it != _models.end(); ++it)
			{
				for (auto _it = it->second.begin(); _it != it->second.end(); ++_it)
				{
					delete* _it;
				}
			}
			for (auto it = _cameras.begin(); it != _cameras.end(); ++it)
			{
				delete* it;
			}
			for (auto it = _compositingNodes.begin(); it != _compositingNodes.end(); ++it)
			{
				delete* it;
			}
		}

		void Renderer::UpdateRecurse(
#ifdef __PRE_DEBUG__
			CompositingNode& currentNode,
			unordered_set<CompositingNode*>& visited
#else
			CompositingNode& currentNode
#endif
		)
		{
#ifdef __PRE_DEBUG__
			auto pCurrentNode = &currentNode;

			if (visited.find(pCurrentNode) != visited.end()) {

				throw "Cyclic compositing dependency detected";
			}

			visited.insert(pCurrentNode);
#endif

			auto& dependencies = currentNode.GetDependencies();
			for (auto it = dependencies.begin(); it != dependencies.end(); ++it)
			{

#ifdef __PRE_DEBUG__
				UpdateRecurse(**it, visited);
#else
				UpdateRecurse(**it);
#endif

			}

			auto itTag = _models.find(currentNode._renderTag);
			if (itTag == _models.end())
			{
				return;
			}

			const glm::mat4* pViewMatrix = &MAT4_IDENTITY;

			auto itCamera = _compositingNodeCameraPairs.find(pCurrentNode);
			if (itCamera != _compositingNodeCameraPairs.end())
			{
				pViewMatrix = &itCamera->second->GetViewProjectionMatrix();
			}
			auto& viewMatrix = *pViewMatrix;

			CompositingTarget::Bind(&currentNode._compositingTarget);
			auto& tagModelSet = itTag->second;
			for (auto it = tagModelSet.begin(); it != tagModelSet.end(); ++it)
			{
				auto& model = **it;
				model.Render(viewMatrix);
			}
			CompositingTarget::Bind(nullptr);

#ifdef __PRE_DEBUG__
			visited.erase(pCurrentNode);
#endif
		}
	} // namespace RenderingModule
} // namespace PRE