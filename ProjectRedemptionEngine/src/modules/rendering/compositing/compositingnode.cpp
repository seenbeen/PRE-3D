#include <modules/rendering/compositing/compositingnode.h>

#include <unordered_set>

#include <glad/glad.h>

#include <modules/rendering/compositing/compositingtarget.h>
#include <modules/rendering/shader/rendershaderprogram.h>
#include <modules/rendering/model/rendertexture.h>

namespace PRE
{
	namespace RenderingModule
	{
		CompositingNode::Impl& CompositingNode::Impl::MakeImpl()
		{
			return *(new Impl());
		}

		CompositingNode::Impl::Impl() {}

		CompositingNode::Impl::~Impl() {}

		CompositingNode::CompositingNode(unsigned int renderTag, CompositingTarget& compositingTarget)
			:
			_renderTag(renderTag),
			_compositingTarget(compositingTarget),
			_impl(Impl::MakeImpl()) {}

		CompositingNode::~CompositingNode()
		{
			delete &_impl;
		}

		void CompositingNode::AddDependency(
			CompositingNode& compositingNode
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

		void CompositingNode::RemoveDependency(CompositingNode& compositingNode)
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

		const unordered_set<CompositingNode*>& CompositingNode::GetDependencies()
		{
			return _impl.compositingNodeDependencies;
		}
	} // namespace RenderingModule
} // namespace PRE