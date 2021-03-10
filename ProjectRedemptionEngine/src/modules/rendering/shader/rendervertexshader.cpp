#include <modules/rendering/shader/rendervertexshader.h>

#include <string>

#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::string;

		RenderVertexShader::Impl& RenderVertexShader::Impl::MakeImpl(const std::string& shaderSource)
		{
			const GLchar* cShaderSource = shaderSource.c_str();
			auto shaderId = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(shaderId, 1, &cShaderSource, NULL);
			glCompileShader(shaderId);

#ifdef __PRE_DEBUG__
			// check compile errors, throw on failure
			GLint success;
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				GLchar infoLog[1024];
				glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
				throw string(infoLog);
			}
#endif

			return *(new Impl(shaderId));
		}

		RenderVertexShader::Impl::Impl(GLuint shaderId) : shaderId(shaderId) {}
		
		RenderVertexShader::Impl::~Impl()
		{
			glDeleteShader(shaderId);
		}

		RenderVertexShader::RenderVertexShader(const string& shaderSource)
			: _impl(Impl::MakeImpl(shaderSource)) {}

		RenderVertexShader::~RenderVertexShader()
		{
			delete &_impl;
		}
	} // namespace RenderingModule
} // namespace PRE