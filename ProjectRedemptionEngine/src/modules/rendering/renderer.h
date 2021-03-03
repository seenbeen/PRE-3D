#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <glm/glm.hpp>

#include <glad/glad.h>

#include <modules/rendering/compositing/compositingnode.h>
#include <modules/rendering/rendercamera.h>

// TODO: migrate SDL to its own namespace to reduce global clutter
struct SDL_Window;
typedef void* SDL_GLContext;

namespace PRE
{
	namespace RenderingModule
	{
		class RenderVertexShader;
		class RenderFragmentShader;
		class RenderShaderProgram;

		class RenderMesh;
		class RenderTexture;
		class RenderMaterial;

		class RenderModel;

		using std::list;
		using std::string;
		using std::unordered_map;
		using std::unordered_set;

		class Renderer
		{
			Renderer& operator=(const Renderer&) = delete;
			Renderer(const Renderer&) = delete;

		public:
			static Renderer& MakeRenderer(
				const string& windowTitle,
				unsigned int windowWidth,
				unsigned int windowHeight
			);
			static void SetActiveRenderer(Renderer& renderer);
			static void ShutdownRenderer(Renderer& renderer);

			void Update();

			CompositingNode& AllocateCompositingNode(
				unsigned int renderTag,
				unsigned int width,
				unsigned int height
			);
			void AttachRootCompositingNodeDependency(CompositingNode& dependency);
			void DetachRootCompositingNodeDependency(CompositingNode& dependency);
			void AttachCompositingNodeDependency(
				CompositingNode& dependent,
				CompositingNode& dependency
			);
			void DetachCompositingNodeDependency(
				CompositingNode& dependent,
				CompositingNode& dependency
			);
			void DeallocateCompositingNode(CompositingNode& compositingNode);

			RenderCamera& AllocateCamera(
				const RenderCamera::Kind& kind,
				float size,
				float aspectRatio,
				float nearClippingPlane,
				float farClippingPlane
			);
			void DeallocateCamera(RenderCamera& camera);

			void BindCompositingPair(
				RenderCamera& camera,
				CompositingNode& compositingNode
			);

			void UnbindCompositingPair(
				RenderCamera& camera,
				CompositingNode& compositingNode
			);

			const RenderVertexShader& AllocateVertexShader(const string& shaderSource);
			void DeallocateVertexShader(const RenderVertexShader& vertexShader);

			const RenderFragmentShader& AllocateFragmentShader(const string& shaderSource);
			void DeallocateFragmentShader(const RenderFragmentShader& fragmentShader);

			const RenderShaderProgram& AllocateShaderProgram(
				const RenderVertexShader& vertexShader,
				const RenderFragmentShader& fragmentShader
			);
			void DeallocateShaderProgram(const RenderShaderProgram& shaderProgram);

			RenderMesh& AllocateMesh();
			void DeallocateMesh(RenderMesh& mesh);

			RenderTexture& AllocateTexture();
			void DeallocateTexture(RenderTexture& texture);

			RenderMaterial& AllocateMaterial(RenderShaderProgram& shaderProgram);
			void SetMaterialShader(RenderMaterial& material, RenderShaderProgram& shaderProgram);
			void SetMaterialTextureBinding(
				RenderMaterial& material,
				RenderTexture* pTexture,
				GLenum bindUnit
			);
			void SetMaterialTextureBinding(
				RenderMaterial& material,
				CompositingNode& compositingNode,
				CompositingNode::CompositingAttachment attachment,
				GLenum bindUnit
			);
			void DeallocateMaterial(RenderMaterial& material);

			RenderModel& AllocateModel(unsigned int renderTag, RenderMesh& mesh, RenderMaterial& material);
			void SetModelMesh(RenderModel& model, RenderMesh& mesh);
			void SetModelMaterial(RenderModel& model, RenderMaterial& material);
			void DeallocateModel(RenderModel& model);

		private:
			static const glm::mat4 MAT4_IDENTITY;

			SDL_Window& _window;
			SDL_GLContext& _glContext;

			unordered_set<CompositingNode*> _rootCompositingNodes;
			unordered_set<CompositingNode*> _compositingNodes;

			unordered_set<const RenderCamera*> _cameras;

			unordered_map<CompositingNode*, RenderCamera*> _compositingNodeCameraPairs;

#ifdef __PRE_DEBUG__
			// Note: this should be a 1:1 match at all times, used for debugging
			unordered_map<RenderCamera*, CompositingNode*> _cameraCompositingNodePairs;
#endif

			unordered_set<const RenderVertexShader*> _vertexShaders;
			unordered_set<const RenderFragmentShader*> _fragmentShaders;
			unordered_set<const RenderShaderProgram*> _shaderPrograms;

			unordered_set<const RenderMesh*> _meshes;
			unordered_set<const RenderTexture*> _textures;
			unordered_set<const RenderMaterial*> _materials;

			unordered_map<unsigned int, unordered_set<RenderModel*>> _models;

			Renderer(SDL_Window& window, SDL_GLContext& glContext);
			~Renderer();

#ifdef __PRE_DEBUG__
			void UpdateRecurse(CompositingNode& currentNode, unordered_set<CompositingNode*>& visited);
#else
			void UpdateRecurse(CompositingNode& currentNode);
#endif
		};
	} // namespace RenderingModule
} // namespace PRE