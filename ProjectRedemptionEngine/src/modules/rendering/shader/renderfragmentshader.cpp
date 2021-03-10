#include <modules/rendering/shader/renderfragmentshader.h>

#include <string>

#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::string;

		RenderFragmentShader::Impl& RenderFragmentShader::Impl::MakeImpl(const std::string& shaderSource)
		{
			const GLchar* cShaderSource = shaderSource.c_str();
			auto shaderId = glCreateShader(GL_FRAGMENT_SHADER);
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

		RenderFragmentShader::Impl::Impl(GLuint shaderId) : shaderId(shaderId) {}

		RenderFragmentShader::Impl::~Impl()
		{
			glDeleteShader(shaderId);
		}

		RenderFragmentShader::RenderFragmentShader(const string& shaderSource)
			: _impl(Impl::MakeImpl(shaderSource)) {}

		RenderFragmentShader::~RenderFragmentShader()
		{
			delete &_impl;
		}
	} // namespace RenderingModule
} // namespace PRE