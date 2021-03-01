#include <modules/rendering/renderer.h>

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <SDL.h>

#include <glad/glad.h>

#include <modules/rendering/shader/rendervertexshader.h>
#include <modules/rendering/shader/renderfragmentshader.h>
#include <modules/rendering/shader/rendershaderprogram.h>

#include <modules/rendering/compositing/compositingtarget.h>
#include <modules/rendering/compositing/compositingnode.h>

#include <modules/rendering/camera/camera.h>
#include <modules/rendering/camera/orthographiccamera.h>
#include <modules/rendering/camera/perspectivecamera.h>

#include <modules/rendering/model/rendermodel.h>
#include <modules/rendering/model/rendermesh.h>
#include <modules/rendering/model/rendertexture.h>
#include <modules/rendering/model/rendermaterial.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::list;
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
				SDL_WINDOW_OPENGL
			);

			if (pWindow == nullptr)
			{
				throw string("Unable to create SDL window. ") + SDL_GetError();
			}

			auto glContext = SDL_GL_CreateContext(pWindow);

			////Use Vsync
			//if (SDL_GL_SetSwapInterval(1) < 0)
			//{
			//	printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			//}

			if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
				throw "Failed to initialize GLAD";
			}

			return *(new Renderer(*pWindow, glContext));
		}

		void Renderer::SetActiveRenderer(Renderer& renderer)
		{
			SDL_GL_MakeCurrent(&renderer._window, renderer._glContext);
		}

		void Renderer::ShutdownRenderer(Renderer& renderer)
		{
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
				auto& currentNode = **it;
#ifdef __PRE_DEBUG__
				UpdateRecurse(currentNode, visited);
#else
				UpdateRecurse(currentNode);
#endif

				auto itTag = _models.find(currentNode._renderTag);
				if (itTag == _models.end())
				{
					continue;
				}

				CompositingTarget::Bind(nullptr);
				auto& tagModelList = itTag->second;
				for (auto it = tagModelList.begin(); it != tagModelList.end(); ++it)
				{
					auto& model = **it;
					model.Render(currentNode._camera);
				}
			}

			SDL_GL_SwapWindow(&_window);
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
				throw "Cannot deallocate unknown RenderVertexShader";
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
				throw "Cannot deallocate unknown RenderFragmentShader";
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
				throw "Cannot deallocate unknown RenderShaderProgram";
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
				throw "Cannot deallocate unknown RenderMesh";
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
				throw "Cannot deallocate unknown RenderTexture";
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
				throw "Cannot operate on unknown RenderMaterial";
			}
			if (_shaderPrograms.find(pShaderProgram) == _shaderPrograms.end())
			{
				throw "Cannot set unknown ShaderProgram to RenderMaterial";
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
				throw "Cannot operate on unknown RenderMaterial";
			}
			if (pTexture != nullptr && _textures.find(pTexture) == _textures.end())
			{
				throw "Cannot bind unknown RenderTexture to RenderMaterial";
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
				throw "Cannot operate on unknown RenderMaterial";
			}
			if (_compositingNodes.find(&compositingNode) == _compositingNodes.end())
			{
				throw "Cannot bind unknown CompositingNode to RenderMaterial";
			}
#endif

			material.SetTextureBinding(&compositingNode.GetAttachment(attachment), bindUnit);
		}

		void Renderer::DeallocateMaterial(RenderMaterial& material)
		{
			auto pMaterial = &material;

#ifdef __PRE_DEBUG__
			if (_materials.find(pMaterial) == _materials.end())
			{
				throw "Cannot deallocate unknown RenderMaterial";
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
				throw "Cannot allocate Model with unknown RenderMesh";
			}
			if (_materials.find(&material) == _materials.end())
			{
				throw "Cannot allocate Model with unknown RenderMaterial";
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
				throw "Cannot operate on unknown RenderModel";
			}
			auto& tagSet = itTag->second;
			if (tagSet.find(&model) == tagSet.end())
			{
				throw "Cannot operate on unknown RenderModel";
			}
			if (_meshes.find(&mesh) == _meshes.end())
			{
				throw "Cannot set unknown RenderMesh to Model";
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
				throw "Cannot operate on unknown RenderModel";
			}
			auto& tagSet = itTag->second;
			if (tagSet.find(&model) == tagSet.end())
			{
				throw "Cannot operate on unknown RenderModel";
			}
			if (_materials.find(&material) == _materials.end())
			{
				throw "Cannot set unknown RenderMaterial to Model";
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
				throw "Cannot deallocate unknown RenderModel";
			}
#endif

			auto& tagModelList = itTag->second;

#ifdef __PRE_DEBUG__
			if (tagModelList.find(pModel) == tagModelList.end())
			{
				throw "Cannot deallocate unknown RenderModel";
			}
#endif

			tagModelList.erase(pModel);
			delete pModel;
		}

		OrthographicCamera& Renderer::AllocateOrthographicCamera(
			float size,
			float aspectRatio,
			float renderDistance
		)
		{
			auto camera = new OrthographicCamera(size, aspectRatio, renderDistance);
			_cameras.insert(camera);
			return *camera;
		}

		PerspectiveCamera& Renderer::AllocatePerspectiveCamera(
			float size,
			float aspectRatio,
			float nearClippingPlane,
			float farClippingPlane
		)
		{
			auto camera = new PerspectiveCamera(
				size,
				aspectRatio,
				nearClippingPlane,
				farClippingPlane
			);
			_cameras.insert(camera);
			return *camera;
		}

		void Renderer::DeallocateCamera(Camera& camera)
		{
			auto pCamera = &camera;

#ifdef __PRE_DEBUG__
			if (_cameras.find(pCamera) == _cameras.end())
			{
				throw "Cannot deallocate unknown Camera";
			}
#endif

			_cameras.erase(pCamera);
			delete pCamera;
		}

		CompositingNode& Renderer::AllocateCompositingNode(
			const Camera& camera,
			unsigned int renderTag,
			unsigned int width,
			unsigned int height
		)
		{
#ifdef __PRE_DEBUG__
			if (_cameras.find(&camera) == _cameras.end())
			{
				throw "Cannot composite using unknown Camera";
			}
#endif

			auto compositingNode = new CompositingNode(camera, renderTag, width, height);
			_compositingNodes.insert(compositingNode);
			return *compositingNode;
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
#endif

			_compositingNodes.erase(pCompositingNode);
			delete pCompositingNode;
		}

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

			currentNode.BindTarget();
			auto& tagModelList = itTag->second;
			for (auto it = tagModelList.begin(); it != tagModelList.end(); ++it)
			{
				auto& model = **it;
				model.Render(currentNode._camera);
			}
			CompositingTarget::Bind(nullptr);

#ifdef __PRE_DEBUG__
			visited.erase(pCurrentNode);
#endif
		}
	} // namespace RenderingModule
} // namespace PRE