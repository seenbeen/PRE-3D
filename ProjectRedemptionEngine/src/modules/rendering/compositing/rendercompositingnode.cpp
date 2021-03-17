#include <modules/rendering/compositing/rendercompositingnode.h>

#include <algorithm>
#include <list>
#include <vector>

#include <modules/rendering/compositing/rendercompositingtarget.h>

#ifdef __PRE_DEBUG__
#include <assert.h>
#endif

namespace PRE
{
	namespace RenderingModule
	{
		using std::list;
		using std::vector;

		void RenderCompositingNode::RenderComposition::SetCamera(RenderCamera* pCamera)
		{
			this->pCamera = pCamera;
		}

		void RenderCompositingNode::RenderComposition::SetCompositingTarget(RenderCompositingTarget* pCompositingTarget)
		{
			this->pCompositingTarget = pCompositingTarget;
		}

		void RenderCompositingNode::RenderComposition::AddModel(RenderModel& model)
		{
#ifdef __PRE_DEBUG__
			assert(std::find(models.begin(), models.end(), &model) == models.end());
#endif

			models.push_back(&model);
		}

		void RenderCompositingNode::RenderComposition::Clear()
		{
			clear = true;
		}

		RenderCompositingNode::RenderComposition::RenderComposition()
			:
			pCamera(nullptr),
			pCompositingTarget(nullptr),
			clear(false) {}

		RenderCompositingNode::RenderComposition::~RenderComposition() {}

		RenderCompositingNode::Impl& RenderCompositingNode::Impl::MakeImpl(
			RenderComposition& composition,
			OnRender onRender,
			void* vContext
		)
		{
			return *(new Impl(composition, onRender, vContext));
		}

		RenderCompositingNode::Impl::Impl(
			RenderComposition& composition,
			OnRender onRender,
			void* vContext
		)
			:
			composition(composition),
			onRender(onRender),
			vContext(vContext) {}

		RenderCompositingNode::Impl::~Impl() {}

		RenderCompositingNode::RenderCompositingNode(
			OnRender onRender,
			void* vContext
		)
			:
			_impl(
				Impl::MakeImpl(
					*(new RenderComposition()),
					onRender,
					vContext
				)
			) {}

		RenderCompositingNode::~RenderCompositingNode()
		{
			delete &_impl.composition;
			delete &_impl;
		}

		void RenderCompositingNode::AddDependency(
			RenderCompositingNode& compositingNode
		)
		{
			auto pCompositingNode = &compositingNode;

#ifdef __PRE_DEBUG__
			auto it = std::find(
				_impl.compositingNodeDependencies.begin(),
				_impl.compositingNodeDependencies.end(),
				pCompositingNode
			);
			assert(it == _impl.compositingNodeDependencies.end());
#endif

			_impl.compositingNodeDependencies.push_back(pCompositingNode);
		}

		void RenderCompositingNode::RemoveDependency(RenderCompositingNode& compositingNode)
		{
			auto pCompositingNode = &compositingNode;
			auto it = std::find(
				_impl.compositingNodeDependencies.begin(),
				_impl.compositingNodeDependencies.end(),
				pCompositingNode
			);

#ifdef __PRE_DEBUG__
			assert(it != _impl.compositingNodeDependencies.end());
#endif

			_impl.compositingNodeDependencies.erase(it);
		}

		const RenderCompositingNode::RenderComposition& RenderCompositingNode::GetRenderComposition()
		{
			_impl.composition.models.clear();
			_impl.composition.pCamera = nullptr;
			_impl.composition.pCompositingTarget = nullptr;
			_impl.composition.clear = false;
			_impl.onRender(_impl.composition, _impl.vContext);

			return _impl.composition;
		}

		const list<RenderCompositingNode*>& RenderCompositingNode::GetDependencies()
		{
			return _impl.compositingNodeDependencies;
		}
	} // namespace RenderingModule
} // namespace PRE