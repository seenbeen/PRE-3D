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
		void RenderShaderProgram::SetBool(const string& name, bool value) const
		{
			glUniform1i(glGetUniformLocation(_programId, name.c_str()), (int)value);
		}

		void RenderShaderProgram::SetInt(const string& name, int value) const
		{
			glUniform1i(glGetUniformLocation(_programId, name.c_str()), value);
		}

		void RenderShaderProgram::SetFloat(const string& name, float value) const
		{
			glUniform1f(glGetUniformLocation(_programId, name.c_str()), value);
		}

		void RenderShaderProgram::SetVec2(const string& name, const glm::vec2& value) const
		{
			glUniform2fv(glGetUniformLocation(_programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetVec3(const string& name, const glm::vec3& value) const
		{
			glUniform3fv(glGetUniformLocation(_programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetVec4(const string& name, const glm::vec4& value) const
		{
			glUniform4fv(glGetUniformLocation(_programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetMat2(const string& name, const glm::mat2& value) const
		{
			glUniformMatrix2fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
		}

		void RenderShaderProgram::SetMat3(const string& name, const glm::mat3& value) const
		{
			glUniformMatrix3fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
		}

		void RenderShaderProgram::SetMat4(const string& name, const glm::mat4& value) const
		{
			glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
		}

		void RenderShaderProgram::Use() const
		{
			glUseProgram(_programId);
		}

		RenderShaderProgram::RenderShaderProgram(
			const RenderVertexShader& vertexShader,
			const RenderFragmentShader& fragmentShader
		)
			: _programId(glCreateProgram())
		{
			glAttachShader(_programId, vertexShader._shaderId);
			glAttachShader(_programId, vertexShader._shaderId);
			glLinkProgram(_programId);

			// validate link status; just throw on failure
			GLint success;
			glGetProgramiv(_programId, GL_LINK_STATUS, &success);
			if (!success)
			{
				GLchar infoLog[1024];
				glGetProgramInfoLog(_programId, 1024, NULL, infoLog);
				throw string(infoLog);
			}
		}

		RenderShaderProgram::~RenderShaderProgram()
		{
			glDeleteProgram(_programId);
		}
	} // namespace RenderingModule
} // namespace PRE