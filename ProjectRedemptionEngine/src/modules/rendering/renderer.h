#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <SDL.h>

#include <glad/glad.h>

#include <modules/rendering/compositing/compositingnode.h>

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

		class Camera;
		class OrthographicCamera;
		class PerspectiveCamera;

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
			
			OrthographicCamera& AllocateOrthographicCamera(
				float size,
				float aspectRatio,
				float renderDistance
			);
			PerspectiveCamera& AllocatePerspectiveCamera(
				float size,
				float aspectRatio,
				float nearClippingPlane,
				float farClippingPlane
			);
			void DeallocateCamera(Camera& camera);

			CompositingNode& AllocateCompositingNode(
				const Camera& camera,
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

		private:
			SDL_Window& _window;
			SDL_GLContext& _glContext;

			unordered_set<CompositingNode*> _rootCompositingNodes;

			unordered_set<RenderVertexShader const*> _vertexShaders;
			unordered_set<RenderFragmentShader const*> _fragmentShaders;
			unordered_set<RenderShaderProgram const*> _shaderPrograms;

			unordered_set<RenderMesh*> _meshes;
			unordered_set<RenderTexture*> _textures;
			unordered_set<RenderMaterial*> _materials;

			unordered_map<unsigned int, unordered_set<RenderModel*>> _models;

			unordered_set<const Camera*> _cameras;

			unordered_set<CompositingNode*> _compositingNodes;

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