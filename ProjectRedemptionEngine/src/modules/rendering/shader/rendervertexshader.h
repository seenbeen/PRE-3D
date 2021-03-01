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

		class RenderVertexShader
		{
			RenderVertexShader& operator=(const RenderVertexShader&) = delete;
			RenderVertexShader(const RenderVertexShader&) = delete;

			friend class Renderer;
			friend class RenderShaderProgram;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderVertexShader;
				friend class RenderShaderProgram;

			private:
				const GLuint shaderId;

				static Impl& MakeImpl(const string& shaderSource);

				Impl(GLuint shaderId);
				~Impl();
			};

		private:
			Impl& _impl;

			RenderVertexShader(const string& shaderSource);
			~RenderVertexShader();
		};
	} // namespace RenderingModule
} // namespace PRE