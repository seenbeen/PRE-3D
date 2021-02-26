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

		private:
			const GLuint _shaderId;

			RenderFragmentShader(const string& shaderSource);
			~RenderFragmentShader();
		};
	} // namespace RenderingModule
} // namespace PRE