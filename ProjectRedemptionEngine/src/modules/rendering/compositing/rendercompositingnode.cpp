#include <modules/rendering/compositing/rendercompositingnode.h>

#include <unordered_set>

#include <modules/rendering/compositing/rendercompositingtarget.h>
#include <modules/rendering/model/rendertexture.h>

#ifdef __PRE_DEBUG__
#include <assert.h>
#endif

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

		RenderCompositingNode::RenderCompositingNode(unsigned int tagGroup, RenderCompositingTarget* pCompositingTarget)
			:
			_tagGroup(tagGroup),
			_pCompositingTarget(pCompositingTarget),
			_impl(Impl::MakeImpl()) {}

		RenderCompositingNode::~RenderCompositingNode()
		{
			delete &_impl;
		}

		RenderTexture& RenderCompositingNode::GetPosition()
		{
			return _pCompositingTarget->GetPosition();
		}

		RenderTexture& RenderCompositingNode::GetNormals()
		{
			return _pCompositingTarget->GetNormals();
		}

		RenderTexture& RenderCompositingNode::GetAlbedoSpecular()
		{
			return _pCompositingTarget->GetAlbedoSpecular();
		}

		void RenderCompositingNode::AddDependency(
			RenderCompositingNode& compositingNode
		)
		{
			auto pCompositingNode = &compositingNode;

#ifdef __PRE_DEBUG__
			auto it = _impl.compositingNodeDependencies.find(pCompositingNode);
			assert(it == _impl.compositingNodeDependencies.end());
#endif

			_impl.compositingNodeDependencies.insert(pCompositingNode);
		}

		void RenderCompositingNode::RemoveDependency(RenderCompositingNode& compositingNode)
		{
			auto pCompositingNode = &compositingNode;
			auto it = _impl.compositingNodeDependencies.find(pCompositingNode);

#ifdef __PRE_DEBUG__
			assert(it != _impl.compositingNodeDependencies.end());
#endif

			_impl.compositingNodeDependencies.erase(it);
		}

		const unordered_set<RenderCompositingNode*>& RenderCompositingNode::GetDependencies()
		{
			return _impl.compositingNodeDependencies;
		}
	} // namespace RenderingModule
} // namespace PRE