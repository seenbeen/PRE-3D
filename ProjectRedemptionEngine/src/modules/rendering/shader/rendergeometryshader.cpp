#include <modules/rendering/shader/rendergeometryshader.h>

#include <string>

#include <glad/glad.h>

#ifdef __PRE_DEBUG__
#include <assert.h>
#include <iostream>
#endif

namespace PRE
{
	namespace RenderingModule
	{
		using std::string;

		RenderGeometryShader::Impl& RenderGeometryShader::Impl::MakeImpl(const std::string& shaderSource)
		{
			const GLchar* cShaderSource = shaderSource.c_str();
			auto shaderId = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(shaderId, 1, &cShaderSource, NULL);
			glCompileShader(shaderId);

#ifdef __PRE_DEBUG__
			// check compile errors
			GLint success;
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				GLchar infoLog[1024];
				glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
				std::cout << string(infoLog) << std::endl;
				assert(false);
			}
#endif

			return *(new Impl(shaderId));
		}

		RenderGeometryShader::Impl::Impl(GLuint shaderId) : shaderId(shaderId) {}

		RenderGeometryShader::Impl::~Impl()
		{
			glDeleteShader(shaderId);
		}

		RenderGeometryShader::RenderGeometryShader(const string& shaderSource)
			: _impl(Impl::MakeImpl(shaderSource)) {}

		RenderGeometryShader::~RenderGeometryShader()
		{
			delete& _impl;
		}
	} // namespace RenderingModule
} // namespace PRE