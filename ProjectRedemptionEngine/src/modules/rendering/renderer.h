#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace PRE
{
	namespace RenderingModule
	{
		class RenderVertexShader;
		class RenderFragmentShader;
		class RenderShaderProgram;

		class CompositingNode;
		class CompositingTarget;
		class Camera;
		class RenderModel;

		using std::list;
		using std::string;
		using std::unordered_map;
		using std::unordered_set;

		class Renderer
		{
			Renderer& operator=(const Renderer&) = delete;
			Renderer(const Renderer&) = delete;

			friend class CompositingNode;

		public:
			Renderer();
			~Renderer();

			void Initialize();
			void Update();
			void Shutdown();

			const RenderVertexShader& AllocateVertexShader(const string& shaderSource);
			void DeallocateVertexShader(const RenderVertexShader& vertexShader);

			const RenderFragmentShader& AllocateFragmentShader(const string& shaderSource);
			void DeallocateFragmentShader(const RenderFragmentShader& fragmentShader);

			const RenderShaderProgram& AllocateShaderProgram(
				const RenderVertexShader& vertexShader,
				const RenderFragmentShader& fragmentShader
			);
			void DeallocateShaderProgram(const RenderShaderProgram& shaderProgram);

			RenderModel& AllocateRenderModel();
			void DeallocateRenderModel(RenderModel& renderModel);

			CompositingNode& AllocateCompositingNode();
			void AttachCompositingNode(CompositingNode& compositingNode, CompositingNode& dependency);
			void DetachCompositingNodeDependency(CompositingNode& compositingNode, CompositingNode& dependency);
			void DeallocateCompositingNode(CompositingNode& compositingNode);

			CompositingTarget& AllocateCompositingTarget();
			void DeallocateCompositingTarget(CompositingTarget& compositingTarget);

		private:
			unordered_set<RenderVertexShader const*> _vertexShaders;
			unordered_set<RenderFragmentShader const*> _fragmentShaders;
			unordered_set<RenderShaderProgram const*> _shaderPrograms;

			unordered_map<CompositingNode*, list<CompositingNode>*> _compositingGraph;

			void Render(Camera& camera, CompositingTarget& compositingTarget);
		};
	} // namespace RenderingModule
} // namespace PRE