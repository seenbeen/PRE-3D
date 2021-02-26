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

		private:
			const GLuint _shaderId;

			RenderVertexShader(const string& shaderSource);
			~RenderVertexShader();
		};
	} // namespace RenderingModule
} // namespace PRE