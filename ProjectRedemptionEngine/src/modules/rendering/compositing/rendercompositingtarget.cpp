#include <modules/rendering/compositing/rendercompositingtarget.h>

#include <glad/glad.h>

#include <modules/rendering/texture/rendertexture.h>

#ifdef __PRE_DEBUG__
#include <assert.h>
#endif

namespace PRE
{
	namespace RenderingModule
	{
		RenderCompositingTarget::Impl& RenderCompositingTarget::Impl::MakeImpl(
			unsigned int width,
			unsigned int height
		)
		{
			GLuint gBufferId;
			glGenFramebuffers(1, &gBufferId);
			glBindFramebuffer(GL_FRAMEBUFFER, gBufferId);

			// this is kinda hack-y... we're using objects to set
			// gl state and then doing some manual tweaking.
			// TODO: maybe clean up later?

			auto target = new RenderTexture(RenderTexture::TextureKind::STANDARD);
			target->Bind();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
			target->BindTarget(GL_COLOR_ATTACHMENT0);

			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			unsigned int rboDepth;
			glGenRenderbuffers(1, &rboDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

#ifdef __PRE_DEBUG__
			assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
#endif

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return *(new Impl(gBufferId, *target));
		}

		RenderCompositingTarget::Impl& RenderCompositingTarget::Impl::MakeImpl(
			unsigned int rightWidth, unsigned int rightHeight,
			unsigned int leftWidth, unsigned int leftHeight,
			unsigned int topWidth, unsigned int topHeight,
			unsigned int bottomWidth, unsigned int bottomHeight,
			unsigned int frontWidth, unsigned int frontHeight,
			unsigned int backWidth, unsigned int backHeight
		)
		{

		}

		RenderCompositingTarget::Impl::Impl(
			GLuint gBufferId,
			RenderTexture& target
		)
			:
			gBufferId(gBufferId),
			target(target) {}

		RenderCompositingTarget::Impl::~Impl()
		{
			delete &target;
			glDeleteFramebuffers(1, &gBufferId);
		}

		RenderCompositingTarget::RenderCompositingTarget(unsigned int width, unsigned int height)
			:
			_impl(Impl::MakeImpl(width, height)) {}

		RenderCompositingTarget::RenderCompositingTarget(
			unsigned int rightWidth, unsigned int rightHeight,
			unsigned int leftWidth, unsigned int leftHeight,
			unsigned int topWidth, unsigned int topHeight,
			unsigned int bottomWidth, unsigned int bottomHeight,
			unsigned int frontWidth, unsigned int frontHeight,
			unsigned int backWidth, unsigned int backHeight
		)
			:
			_impl(Impl::MakeImpl(
				rightWidth, rightHeight,
				leftWidth, leftHeight,
				topWidth, topHeight,
				bottomWidth, bottomHeight,
				frontWidth, frontHeight,
				backWidth, backHeight
			)) {}

		RenderCompositingTarget::~RenderCompositingTarget()
		{
			delete &_impl;
		}

		RenderTexture& RenderCompositingTarget::GetTarget()
		{
			return _impl.target;
		}

		void RenderCompositingTarget::Bind(RenderCompositingTarget* pCompositingTarget)
		{
			if (pCompositingTarget == nullptr)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, pCompositingTarget->_impl.gBufferId);
			}
		}
	} // namespace RenderingModule
} // namespace PRE
