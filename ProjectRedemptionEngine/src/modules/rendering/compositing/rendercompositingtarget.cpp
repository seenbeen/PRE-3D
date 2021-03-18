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

			GLuint renderBufferId;
			glGenRenderbuffers(1, &renderBufferId);
			glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

#ifdef __PRE_DEBUG__
			assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
#endif

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return *(new Impl(width, height, bufferId, renderBufferId, *target));
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
			unsigned int width,
			unsigned int height,
			GLuint bufferId,
			GLuint renderBufferId,
			RenderTexture& target
		)
			:
			width(width),
			height(height),
			bufferId(bufferId),
			renderBufferId(renderBufferId),
			target(target) {}

		RenderCompositingTarget::Impl::~Impl()
		{
			delete &target;
			glDeleteRenderbuffers(1, &renderBufferId);
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

		// NOTE: THIS DOES NOT RETURN THE VIEWPORT TO NORMAL IF NULL IS PASSED
		void RenderCompositingTarget::Bind(RenderCompositingTarget* pCompositingTarget)
		{
			if (pCompositingTarget == nullptr)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
			}
			else
			{
				// TODO: switch on buffer type
				glBindFramebuffer(GL_FRAMEBUFFER, pCompositingTarget->_impl.bufferId);
				glViewport(0, 0, pCompositingTarget->_impl.width, pCompositingTarget->_impl.height);
			}
		}
	} // namespace RenderingModule
} // namespace PRE
