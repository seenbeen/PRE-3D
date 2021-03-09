#include <modules/rendering/shader/rendershaderprogram.h>

#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <modules/rendering/shader/rendervertexshader.h>
#include <modules/rendering/shader/renderfragmentshader.h>

namespace PRE
{
	namespace RenderingModule
	{
		RenderShaderProgram::Impl& RenderShaderProgram::Impl::MakeImpl(
			const RenderVertexShader& vertexShader,
			const RenderFragmentShader& fragmentShader
		)
		{
			auto programId = glCreateProgram();
			glAttachShader(programId, vertexShader._impl.shaderId);
			glAttachShader(programId, fragmentShader._impl.shaderId);
			glLinkProgram(programId);

#ifdef __PRE_DEBUG__
			// validate link status; just throw on failure
			GLint success;
			glGetProgramiv(programId, GL_LINK_STATUS, &success);
			if (!success)
			{
				GLchar infoLog[1024];
				glGetProgramInfoLog(programId, 1024, NULL, infoLog);
				throw string(infoLog);
			}
#endif

			return *(new Impl(programId));
		}

		RenderShaderProgram::Impl::Impl(GLuint programId) : programId(programId) {}

		RenderShaderProgram::Impl::~Impl()
		{
			glDeleteProgram(programId);
		}

		void RenderShaderProgram::SetBool(const string& name, bool value)
		{
			Bind();
			glUniform1i(glGetUniformLocation(_impl.programId, name.c_str()), (int)value);
		}

		void RenderShaderProgram::SetInt(const string& name, int value)
		{
			Bind();
			glUniform1i(glGetUniformLocation(_impl.programId, name.c_str()), value);
		}

		void RenderShaderProgram::SetFloat(const string& name, float value)
		{
			Bind();
			glUniform1f(glGetUniformLocation(_impl.programId, name.c_str()), value);
		}

		void RenderShaderProgram::SetVec2(const string& name, const glm::vec2& value)
		{
			Bind();
			glUniform2fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetVec3(const string& name, const glm::vec3& value)
		{
			Bind();
			glUniform3fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetVec4(const string& name, const glm::vec4& value)
		{
			Bind();
			glUniform4fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetIVec2(const string& name, const glm::ivec2& value)
		{
			Bind();
			glUniform2iv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetIVec3(const string& name, const glm::ivec3& value)
		{
			Bind();
			glUniform3iv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetIVec4(const string& name, const glm::ivec4& value)
		{
			Bind();
			glUniform4iv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetMat2(const string& name, const glm::mat2& value)
		{
			Bind();
			glUniformMatrix2fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
		}

		void RenderShaderProgram::SetMat3(const string& name, const glm::mat3& value)
		{
			Bind();
			glUniformMatrix3fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
		}

		void RenderShaderProgram::SetMat4(const string& name, const glm::mat4& value)
		{
			Bind();
			glUniformMatrix4fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
		}

		void RenderShaderProgram::SetVec2(const string& name, const glm::vec2* values, unsigned int nValues)
		{
			Bind();
			glUniform2fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetVec3(const string& name, const glm::vec3* values, unsigned int nValues)
		{
			Bind();
			glUniform3fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetVec4(const string& name, const glm::vec4* values, unsigned int nValues)
		{
			Bind();
			glUniform4fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetIVec2(const string& name, const glm::ivec2* values, unsigned int nValues)
		{
			Bind();
			glUniform2iv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetIVec3(const string& name, const glm::ivec3* values, unsigned int nValues)
		{
			Bind();
			glUniform3iv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetIVec4(const string& name, const glm::ivec4* values, unsigned int nValues)
		{
			Bind();
			glUniform4iv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetMat2(const string& name, const glm::mat2* value, unsigned int nValues)
		{
			Bind();
			glUniformMatrix2fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, GL_FALSE, &value[0][0][0]);
		}

		void RenderShaderProgram::SetMat3(const string& name, const glm::mat3* value, unsigned int nValues)
		{
			Bind();
			glUniformMatrix3fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, GL_FALSE, &value[0][0][0]);
		}

		void RenderShaderProgram::SetMat4(const string& name, const glm::mat4* value, unsigned int nValues)
		{
			Bind();
			glUniformMatrix4fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, GL_FALSE, &value[0][0][0]);
		}

		RenderShaderProgram::RenderShaderProgram(
			const RenderVertexShader& vertexShader,
			const RenderFragmentShader& fragmentShader
		)
			:
			_impl(Impl::MakeImpl(vertexShader, fragmentShader)) {}

		RenderShaderProgram::~RenderShaderProgram()
		{
			delete& _impl;
		}

		void RenderShaderProgram::Bind()
		{
			glUseProgram(_impl.programId);
		}
	} // namespace RenderingModule
} // namespace PRE