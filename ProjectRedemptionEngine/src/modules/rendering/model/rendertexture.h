#pragma once
#include <string>

#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		class CompositingTarget;
		class RenderMaterial;
		class Renderer;

		using std::string;

		class RenderTexture
		{
			RenderTexture& operator=(const RenderTexture&) = delete;
			RenderTexture(const RenderTexture&) = delete;

			friend class CompositingTarget;
			friend class RenderMaterial;
			friend class Renderer;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderTexture;

			private:
				const GLuint textureId;
				unsigned int width;
				unsigned int height;
				unsigned int* data;
				bool hasChanged;

				static Impl& MakeImpl();

				Impl(GLuint textureId);
				~Impl();
			};

		public:
			void SetData(
				unsigned int width,
				unsigned int height,
				const unsigned int* const data
			);

		private:
			Impl& _impl;

			RenderTexture();
			~RenderTexture();

			void Bind();
			void BindTarget(GLenum attachment);

			static void SetActive(GLenum textureUnit);
		};
	} // namespace RenderingModule
} // namespace PRE