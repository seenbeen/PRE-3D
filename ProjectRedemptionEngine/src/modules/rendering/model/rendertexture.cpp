#include <modules/rendering/model/rendertexture.h>

#include <cstring>

#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
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
			textureId(textureId),
			width(0),
			height(0),
			data(nullptr),
			hasChanged(false) {}

		RenderTexture::Impl::~Impl()
		{
			glDeleteTextures(1, &textureId);
		}

		void RenderTexture::SetData(
			unsigned int width,
			unsigned int height,
			const unsigned int* const data
		)
		{
			auto total = (int)(width * height);

#ifdef __PRE_DEBUG__
			if (total < 0) {
				throw "texture data overflow";
			}
#endif

			delete[] _impl.data;
			_impl.data = new unsigned int[total];
			std::memcpy(_impl.data, data, total * sizeof(unsigned int));
			_impl.width = width;
			_impl.height = height;
			_impl.hasChanged = true;
		}

		RenderTexture::RenderTexture()
			: _impl(Impl::MakeImpl()) {}

		RenderTexture::~RenderTexture()
		{
			delete &_impl;
		}

		void RenderTexture::Bind()
		{
			glBindTexture(GL_TEXTURE_2D, _impl.textureId);
			if (_impl.hasChanged)
			{
				_impl.hasChanged = false;
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RGBA,
					_impl.width,
					_impl.height,
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					_impl.data
				);
			}
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