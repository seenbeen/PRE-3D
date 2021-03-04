#pragma once
#include <unordered_set>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class CompositingTarget;

		using std::unordered_set;

		class RenderCompositingNode
		{
			RenderCompositingNode& operator=(const RenderCompositingNode&) = delete;
			RenderCompositingNode(const RenderCompositingNode&) = delete;

			friend class Renderer;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderCompositingNode;

			private:
				unordered_set<RenderCompositingNode*> compositingNodeDependencies;

				static Impl& MakeImpl();

				Impl();
				~Impl();
			};

		private:
			const unsigned int _tagGroup;
			CompositingTarget* const _pCompositingTarget;

			Impl& _impl;

			RenderCompositingNode(unsigned int tagGroup, CompositingTarget* pCompositingTarget);
			~RenderCompositingNode();

			void AddDependency(RenderCompositingNode& compositingNode);
			void RemoveDependency(RenderCompositingNode& compositingNode);

			const unordered_set<RenderCompositingNode*>& GetDependencies();
		};
	} // namespace RenderingModule
} // namespace PRE