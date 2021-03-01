#pragma once
#include <string>

#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderShaderProgram;

		using std::string;

		class RenderFragmentShader
		{
			RenderFragmentShader& operator=(const RenderFragmentShader&) = delete;
			RenderFragmentShader(const RenderFragmentShader&) = delete;

			friend class Renderer;
			friend class RenderShaderProgram;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderFragmentShader;
				friend class RenderShaderProgram;

			private:
				const GLuint shaderId;

				static Impl& MakeImpl(const string& shaderSource);

				Impl(GLuint shaderId);
				~Impl();
			};

		private:
			Impl& _impl;

			RenderFragmentShader(const string& shaderSource);
			~RenderFragmentShader();
		};
	} // namespace RenderingModule
} // namespace PRE