#pragma once
#include <unordered_set>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderCompositingTarget;

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
			RenderCompositingTarget* const _pCompositingTarget;

			Impl& _impl;

			RenderCompositingNode(unsigned int tagGroup, RenderCompositingTarget* pCompositingTarget);
			~RenderCompositingNode();

			void AddDependency(RenderCompositingNode& compositingNode);
			void RemoveDependency(RenderCompositingNode& compositingNode);

			const unordered_set<RenderCompositingNode*>& GetDependencies();
		};
	} // namespace RenderingModule
} // namespace PRE