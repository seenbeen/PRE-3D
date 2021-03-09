#include <modules/rendering/model/rendertexture.h>

#include <cstring>

#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		RenderTexture::Impl& RenderTexture::Impl::MakeImpl(
			unsigned int width,
			unsigned int height,
			const unsigned char* const data
		)
		{
			GLuint textureId;
			glGenTextures(1, &textureId);

			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				data
			);
			glBindTexture(GL_TEXTURE_2D, GL_NONE);

			return *(new RenderTexture::Impl(textureId));
		}

		RenderTexture::Impl& RenderTexture::Impl::MakeImpl()
		{
			GLuint textureId;
			glGenTextures(1, &textureId);

			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glBindTexture(GL_TEXTURE_2D, GL_NONE);

			return *(new RenderTexture::Impl(textureId));
		}

		RenderTexture::Impl::Impl(GLuint textureId)
			:
			textureId(textureId) {}

		RenderTexture::Impl::~Impl()
		{
			glDeleteTextures(1, &textureId);
		}

		RenderTexture::RenderTexture()
			:
			_impl(Impl::MakeImpl()) {}

		RenderTexture::RenderTexture(
			unsigned int width,
			unsigned int height,
			const unsigned char* data
		)
			:
			_impl(Impl::MakeImpl(width, height, data)) {}

		RenderTexture::~RenderTexture()
		{
			delete &_impl;
		}

		void RenderTexture::Bind()
		{
			glBindTexture(GL_TEXTURE_2D, _impl.textureId);
		}

		void RenderTexture::BindTarget(GLenum attachment)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, _impl.textureId, 0);
		}

		void RenderTexture::SetActive(GLenum textureUnit)
		{
			glActiveTexture(textureUnit);
		}
	}
}