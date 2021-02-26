#include <modules/rendering/shader/rendervertexshader.h>

#include <string>

#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::string;

		RenderVertexShader::RenderVertexShader(const string& shaderSource)
			: _shaderId(glCreateShader(GL_VERTEX_SHADER))
		{
			const GLchar* glCharSource = shaderSource.c_str();
			glShaderSource(_shaderId, 1, &glCharSource, NULL);
			glCompileShader(_shaderId);

			// check compile errors, throw on failure
			GLint success;
			glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				GLchar infoLog[1024];
				glGetProgramInfoLog(_shaderId, 1024, NULL, infoLog);
				throw string(infoLog);
			}
		}

		RenderVertexShader::~RenderVertexShader()
		{
			glDeleteShader(_shaderId);
		}
	} // namespace RenderingModule
} // namespace PRE