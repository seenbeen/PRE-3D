#include <modules/rendering/texture/rendertexture.h>

#include <vector>

#include <glad/glad.h>

#include <iostream>

namespace PRE
{
	namespace RenderingModule
	{
		using std::vector;

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

			return *(new RenderTexture::Impl(TextureKind::STANDARD, textureId));
		}

		RenderTexture::Impl& RenderTexture::Impl::MakeImpl(
			unsigned int rightWidth,
			unsigned int rightHeight,
			const unsigned char* rightData,
			unsigned int leftWidth,
			unsigned int leftHeight,
			const unsigned char* leftData,
			unsigned int topWidth,
			unsigned int topHeight,
			const unsigned char* topData,
			unsigned int bottomWidth,
			unsigned int bottomHeight,
			const unsigned char* bottomData,
			unsigned int frontWidth,
			unsigned int frontHeight,
			const unsigned char* frontData,
			unsigned int backWidth,
			unsigned int backHeight,
			const unsigned char* backData
		)
		{
			GLuint textureId;
			glGenTextures(1, &textureId);

			glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			struct ImageData{
				const unsigned int& width;
				const unsigned int& height;
				const unsigned char* data;

				ImageData(unsigned int& width, unsigned int& height, const unsigned char* data)
					:
					width(width),
					height(height),
					data(data) {}
			};

			ImageData cubeFaces[] {
				ImageData(rightWidth, rightHeight, rightData), 
				ImageData(leftWidth, leftHeight, leftData),
				ImageData(topWidth, topHeight, topData),
				ImageData(bottomWidth, bottomHeight, bottomData),
				ImageData(frontWidth, frontHeight, frontData),
				ImageData(backWidth, backHeight, backData)
			};

			for (auto i = 0; i < 6; ++i)
			{
				auto& face = cubeFaces[i];
				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					GL_RGBA,
					face.width,
					face.height,
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					face.data
				);
			}

			glBindTexture(GL_TEXTURE_CUBE_MAP, GL_NONE);

			return *(new RenderTexture::Impl(TextureKind::CUBE_MAP, textureId));
		}

		RenderTexture::Impl::Impl(const TextureKind& textureKind, GLuint textureId)
			:
			textureKind(textureKind),
			textureId(textureId) {}

		RenderTexture::Impl::~Impl()
		{
			glDeleteTextures(1, &textureId);
		}

		RenderTexture::RenderTexture(
			unsigned int width,
			unsigned int height,
			const unsigned char* data
		)
			:
			_impl(Impl::MakeImpl(width, height, data)) {}

		RenderTexture::RenderTexture(
			unsigned int rightWidth,
			unsigned int rightHeight,
			const unsigned char* rightData,
			unsigned int leftWidth,
			unsigned int leftHeight,
			const unsigned char* leftData,
			unsigned int topWidth,
			unsigned int topHeight,
			const unsigned char* topData,
			unsigned int bottomWidth,
			unsigned int bottomHeight,
			const unsigned char* bottomData,
			unsigned int frontWidth,
			unsigned int frontHeight,
			const unsigned char* frontData,
			unsigned int backWidth,
			unsigned int backHeight,
			const unsigned char* backData
		)
			:
			_impl(Impl::MakeImpl(
				rightWidth, rightHeight, rightData,
				leftWidth, leftHeight, leftData,
				topWidth, topHeight, topData,
				bottomWidth, bottomHeight, bottomData,
				frontWidth, frontHeight, frontData,
				backWidth, backHeight, backData
			)) {}

		RenderTexture::~RenderTexture()
		{
			delete &_impl;
		}

		void RenderTexture::Bind()
		{
			if (_impl.textureKind == Impl::TextureKind::STANDARD)
			{
				glBindTexture(GL_TEXTURE_2D, _impl.textureId);
			}
			else
			{
				glBindTexture(GL_TEXTURE_CUBE_MAP, _impl.textureId);
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