#pragma once
#include <unordered_set>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class CompositingTarget;

		using std::unordered_set;

		class CompositingNode
		{
			CompositingNode& operator=(const CompositingNode&) = delete;
			CompositingNode(const CompositingNode&) = delete;

			friend class Renderer;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class CompositingNode;

			private:
				unordered_set<CompositingNode*> compositingNodeDependencies;

				static Impl& MakeImpl();

				Impl();
				~Impl();
			};

		private:
			const unsigned int _tagGroup;
			CompositingTarget* const _pCompositingTarget;

			Impl& _impl;

			CompositingNode(unsigned int tagGroup, CompositingTarget* pCompositingTarget);
			~CompositingNode();

			void AddDependency(CompositingNode& compositingNode);
			void RemoveDependency(CompositingNode& compositingNode);

			const unordered_set<CompositingNode*>& GetDependencies();
		};
	} // namespace RenderingModule
} // namespace PRE