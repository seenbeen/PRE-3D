#pragma once
#include <list>
#include <vector>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderCamera;
		class RenderCompositingTarget;
		class RenderTexture;
		class RenderModel;

		using std::list;
		using std::vector;

		class RenderCompositingNode
		{
			RenderCompositingNode& operator=(const RenderCompositingNode&) = delete;
			RenderCompositingNode(const RenderCompositingNode&) = delete;

			friend class Renderer;

		public:
			class RenderComposition
			{
				RenderComposition& operator=(const RenderComposition&) = delete;
				RenderComposition(const RenderComposition&) = delete;

				friend class Renderer;
				friend class RenderCompositingNode;

			public:
				void SetCamera(RenderCamera* pCamera);
				void SetCompositingTarget(RenderCompositingTarget* pCompositingTarget);
				void AddModel(RenderModel& model);
				void Clear();

			private:
				RenderCamera* pCamera;
				RenderCompositingTarget* pCompositingTarget;
				vector<RenderModel*> models;
				bool clear;

				RenderComposition();
				~RenderComposition();
			};

			typedef void (*OnRender)(RenderComposition& composition, void* vContext);

		private:
			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderCompositingNode;

			private:
				// shouldn't impact performance as these shouldn't
				// mutate too much; if it becomes a bottleneck,
				// a relatively quickfix would be to
				// map iterators to pointers
				list<RenderCompositingNode*> compositingNodeDependencies;

				RenderComposition& composition;
				OnRender const onRender;
				void* const vContext;

				static Impl& MakeImpl(
					RenderComposition& composition,
					OnRender onRender,
					void* vContext
				);

				Impl(
					RenderComposition& composition,
					OnRender onRender,
					void* vContext
				);
				~Impl();
			};

			Impl& _impl;

			RenderCompositingNode(OnRender onRender, void* vContext);
			~RenderCompositingNode();

			void AddDependency(RenderCompositingNode& compositingNode);
			void RemoveDependency(RenderCompositingNode& compositingNode);

			const RenderComposition& GetRenderComposition();
			const list<RenderCompositingNode*>& GetDependencies();
		};
	} // namespace RenderingModule
} // namespace PRE