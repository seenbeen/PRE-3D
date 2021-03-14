#pragma once
#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;

		class RenderCompositingTarget;
		class RenderMaterial;

		class RenderTexture
		{
			RenderTexture& operator=(const RenderTexture&) = delete;
			RenderTexture(const RenderTexture&) = delete;

			friend class Renderer;
			friend class RenderCompositingTarget;
			friend class RenderMaterial;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderTexture;

			private:
				enum class TextureKind { STANDARD, CUBE_MAP };

				const TextureKind textureKind;

				const GLuint textureId;
				
				static Impl& MakeImpl(
					unsigned int width,
					unsigned int height,
					const unsigned char* data
				);

				static Impl& MakeImpl(
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
				);

				Impl(const TextureKind& textureKind, GLuint textureId);
				~Impl();
			};

		private:
			Impl& _impl;

			RenderTexture(
				unsigned int width,
				unsigned int height,
				const unsigned char* data
			);

			RenderTexture(
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
			);

			~RenderTexture();

			void Bind();
			void BindTarget(GLenum attachment);

			static void SetActive(GLenum textureUnit);
		};
	} // namespace RenderingModule
} // namespace PRE