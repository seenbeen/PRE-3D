#include <modules/rendering/compositing/rendercompositingnode.h>

#include <unordered_set>

#include <modules/rendering/compositing/compositingtarget.h>

namespace PRE
{
	namespace RenderingModule
	{
		RenderCompositingNode::Impl& RenderCompositingNode::Impl::MakeImpl()
		{
			return *(new Impl());
		}

		RenderCompositingNode::Impl::Impl() {}

		RenderCompositingNode::Impl::~Impl() {}

		RenderCompositingNode::RenderCompositingNode(unsigned int tagGroup, CompositingTarget* pCompositingTarget)
			:
			_tagGroup(tagGroup),
			_pCompositingTarget(pCompositingTarget),
			_impl(Impl::MakeImpl()) {}

		RenderCompositingNode::~RenderCompositingNode()
		{
			delete &_impl;
		}

		void RenderCompositingNode::AddDependency(
			RenderCompositingNode& compositingNode
		)
		{
			auto pCompositingNode = &compositingNode;

#ifdef __PRE_DEBUG__
			auto it = _impl.compositingNodeDependencies.find(pCompositingNode);
			if (it != _impl.compositingNodeDependencies.end())
			{
				throw "duplicate CompositingNode dependency detected";
			}
#endif

			_impl.compositingNodeDependencies.insert(pCompositingNode);
		}

		void RenderCompositingNode::RemoveDependency(RenderCompositingNode& compositingNode)
		{
			auto pCompositingNode = &compositingNode;
			auto it = _impl.compositingNodeDependencies.find(pCompositingNode);

#ifdef __PRE_DEBUG__
			if (it == _impl.compositingNodeDependencies.end())
			{
				throw "cannot remove unknown CompositingNode dependency";
			}
#endif

			_impl.compositingNodeDependencies.erase(it);
		}

		const unordered_set<RenderCompositingNode*>& RenderCompositingNode::GetDependencies()
		{
			return _impl.compositingNodeDependencies;
		}
	} // namespace RenderingModule
} // namespace PRE