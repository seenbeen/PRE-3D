#include <modules/rendering/compositing/compositingnode.h>

#include <glad/glad.h>

#include <modules/rendering/shader/rendershaderprogram.h>

#include <modules/rendering/camera/camera.h>

#include <modules/rendering/compositing/compositingtarget.h>

#include <modules/rendering/model/rendertexture.h>

namespace PRE
{
	namespace RenderingModule
	{
		CompositingNode::Impl& CompositingNode::Impl::MakeImpl(unsigned int width, unsigned int height)
		{
			auto target = new CompositingTarget(width, height);
			return *(new Impl(*target));
		}

		CompositingNode::Impl::Impl(CompositingTarget& target)
			:
			target(target) {}

		CompositingNode::Impl::~Impl()
		{
			delete &target;
		}

		CompositingNode::CompositingNode(const Camera& camera, unsigned int renderTag, unsigned int width, unsigned int height)
			:
			_camera(camera),
			_renderTag(renderTag),
			_impl(Impl::MakeImpl(width, height)) {}

		CompositingNode::~CompositingNode()
		{
			delete &_impl;
		}

		void CompositingNode::BindTarget()
		{
			CompositingTarget::Bind(&_impl.target);
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

		RenderTexture& CompositingNode::GetAttachment(CompositingAttachment attachment)
		{
			switch (attachment)
			{
				case CompositingAttachment::POSITION:
					return _impl.target.GetPosition();
				case CompositingAttachment::NORMALS:
					return _impl.target.GetNormals();
				case CompositingAttachment::ALBEDO_SPECULAR:
					return _impl.target.GetAlbedoSpecular();
			}
		}

		const unordered_set<CompositingNode*>& CompositingNode::GetDependencies()
		{
			return _impl.compositingNodeDependencies;
		}
	} // namespace RenderingModule
} // namespace PRE