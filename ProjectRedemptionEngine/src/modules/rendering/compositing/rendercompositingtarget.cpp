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
			GLuint bufferId;
			glGenFramebuffers(1, &bufferId);
			glBindFramebuffer(GL_FRAMEBUFFER, bufferId);

			auto target = new RenderTexture(width, height, nullptr);
			target->Bind();
			target->BindTarget(GL_COLOR_ATTACHMENT0);

			// TODO: rbo leaks
			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

#ifdef __PRE_DEBUG__
			assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
#endif

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return *(new Impl(bufferId, *target));
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
			// TODO: Cubemap framebuffers
		}

		RenderCompositingTarget::Impl::Impl(
			GLuint bufferId,
			RenderTexture& target
		)
			:
			bufferId(bufferId),
			target(target) {}

		RenderCompositingTarget::Impl::~Impl()
		{
			delete &target;
			glDeleteFramebuffers(1, &bufferId);
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
			_impl(
				Impl::MakeImpl(
					rightWidth, rightHeight,
					leftWidth, leftHeight,
					topWidth, topHeight,
					bottomWidth, bottomHeight,
					frontWidth, frontHeight,
					backWidth, backHeight
				)
			) {}

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
				glBindFramebuffer(GL_FRAMEBUFFER, pCompositingTarget->_impl.bufferId);
			}
		}
	} // namespace RenderingModule
} // namespace PRE
