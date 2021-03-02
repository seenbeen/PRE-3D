#include <modules/rendering/compositing/compositingtarget.h>

#include <glad/glad.h>

#include <modules/rendering/model/rendertexture.h>

namespace PRE
{
	namespace RenderingModule
	{
		CompositingTarget::Impl& CompositingTarget::Impl::MakeImpl(
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

			auto position = new RenderTexture();
			position->Bind();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			position->BindTarget(GL_COLOR_ATTACHMENT0);
			
			auto normals = new RenderTexture();
			normals->Bind();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			normals->BindTarget(GL_COLOR_ATTACHMENT1);
			
			auto albedoSpecular = new RenderTexture();
			albedoSpecular->Bind();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			albedoSpecular->BindTarget(GL_COLOR_ATTACHMENT2);

			unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);

			unsigned int rboDepth;
			glGenRenderbuffers(1, &rboDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

#ifdef __PRE_DEBUG__
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				throw "Failed to make framebuffer :<";
			}
#endif

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return *(new Impl(gBufferId, *position, *normals, *albedoSpecular));
		}

		CompositingTarget::Impl::Impl(
			GLuint gBufferId,
			RenderTexture& position,
			RenderTexture& normals,
			RenderTexture& albedoSpecular
		)
			:
			gBufferId(gBufferId),
			position(position),
			normals(normals),
			albedoSpecular(albedoSpecular) {}

		CompositingTarget::Impl::~Impl()
		{
			delete& albedoSpecular;
			delete& normals;
			delete& position;
			glDeleteFramebuffers(1, &gBufferId);
		}

		CompositingTarget::CompositingTarget(unsigned int width, unsigned int height)
			: _impl(Impl::MakeImpl(width, height)) {}

		CompositingTarget::~CompositingTarget()
		{
			delete &_impl;
		}

		RenderTexture& CompositingTarget::GetPosition()
		{
			return _impl.position;
		}

		RenderTexture& CompositingTarget::GetNormals()
		{
			return _impl.normals;
		}

		RenderTexture& CompositingTarget::GetAlbedoSpecular()
		{
			return _impl.albedoSpecular;
		}

		void CompositingTarget::Bind(CompositingTarget* pCompositingTarget)
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