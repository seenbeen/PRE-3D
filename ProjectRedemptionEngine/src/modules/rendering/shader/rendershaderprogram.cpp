#include <modules/rendering/shader/rendershaderprogram.h>

#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <modules/rendering/shader/rendervertexshader.h>
#include <modules/rendering/shader/rendergeometryshader.h>
#include <modules/rendering/shader/renderfragmentshader.h>

#ifdef __PRE_DEBUG__
#include <assert.h>
#endif

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
			// validate link status
			GLint success;
			glGetProgramiv(programId, GL_LINK_STATUS, &success);
			if (!success)
			{
				GLchar infoLog[1024];
				glGetProgramInfoLog(programId, 1024, NULL, infoLog);
				assert(false);
			}
#endif

			return *(new Impl(programId));
		}

		RenderShaderProgram::Impl& RenderShaderProgram::Impl::MakeImpl(
			const RenderVertexShader& vertexShader,
			const RenderGeometryShader& geometryShader,
			const RenderFragmentShader& fragmentShader
		)
		{
			auto programId = glCreateProgram();
			glAttachShader(programId, vertexShader._impl.shaderId);
			glAttachShader(programId, geometryShader._impl.shaderId);
			glAttachShader(programId, fragmentShader._impl.shaderId);
			glLinkProgram(programId);

#ifdef __PRE_DEBUG__
			// validate link status
			GLint success;
			glGetProgramiv(programId, GL_LINK_STATUS, &success);
			if (!success)
			{
				GLchar infoLog[1024];
				glGetProgramInfoLog(programId, 1024, NULL, infoLog);
				assert(false);
			}
#endif

			return *(new Impl(programId));
		}

		RenderShaderProgram::Impl::Impl(GLuint programId)
			:
			programId(programId),
			backFaceCullEnabled(true),
			depthWrite(true),
			depthFunction(DepthFunction::LESS_THAN) {}

		RenderShaderProgram::Impl::~Impl()
		{
			glDeleteProgram(programId);
		}

		void RenderShaderProgram::SetBackFaceCulling(bool enabled)
		{
			_impl.backFaceCullEnabled = enabled;
		}

		void RenderShaderProgram::SetDepthWrite(bool enabled)
		{
			_impl.depthWrite = enabled;
		}

		void RenderShaderProgram::SetDepthFunction(const DepthFunction& depthFunction)
		{
			_impl.depthFunction = depthFunction;
		}

		void RenderShaderProgram::SetBool(const string& name, bool value)
		{
			glUseProgram(_impl.programId);
			glUniform1i(glGetUniformLocation(_impl.programId, name.c_str()), (int)value);
		}

		void RenderShaderProgram::SetInt(const string& name, int value)
		{
			glUseProgram(_impl.programId);
			glUniform1i(glGetUniformLocation(_impl.programId, name.c_str()), value);
		}

		void RenderShaderProgram::SetFloat(const string& name, float value)
		{
			glUseProgram(_impl.programId);
			glUniform1f(glGetUniformLocation(_impl.programId, name.c_str()), value);
		}

		void RenderShaderProgram::SetVec2(const string& name, const glm::vec2& value)
		{
			glUseProgram(_impl.programId);
			glUniform2fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetVec3(const string& name, const glm::vec3& value)
		{
			glUseProgram(_impl.programId);
			glUniform3fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetVec4(const string& name, const glm::vec4& value)
		{
			glUseProgram(_impl.programId);
			glUniform4fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetIVec2(const string& name, const glm::ivec2& value)
		{
			glUseProgram(_impl.programId);
			glUniform2iv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetIVec3(const string& name, const glm::ivec3& value)
		{
			glUseProgram(_impl.programId);
			glUniform3iv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetIVec4(const string& name, const glm::ivec4& value)
		{
			glUseProgram(_impl.programId);
			glUniform4iv(glGetUniformLocation(_impl.programId, name.c_str()), 1, &value[0]);
		}

		void RenderShaderProgram::SetMat2(const string& name, const glm::mat2& value)
		{
			glUseProgram(_impl.programId);
			glUniformMatrix2fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
		}

		void RenderShaderProgram::SetMat3(const string& name, const glm::mat3& value)
		{
			glUseProgram(_impl.programId);
			glUniformMatrix3fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
		}

		void RenderShaderProgram::SetMat4(const string& name, const glm::mat4& value)
		{
			glUseProgram(_impl.programId);
			glUniformMatrix4fv(glGetUniformLocation(_impl.programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
		}

		void RenderShaderProgram::SetVec2(const string& name, const glm::vec2* values, unsigned int nValues)
		{
			glUseProgram(_impl.programId);
			glUniform2fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetVec3(const string& name, const glm::vec3* values, unsigned int nValues)
		{
			glUseProgram(_impl.programId);
			glUniform3fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetVec4(const string& name, const glm::vec4* values, unsigned int nValues)
		{
			glUseProgram(_impl.programId);
			glUniform4fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetIVec2(const string& name, const glm::ivec2* values, unsigned int nValues)
		{
			glUseProgram(_impl.programId);
			glUniform2iv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetIVec3(const string& name, const glm::ivec3* values, unsigned int nValues)
		{
			glUseProgram(_impl.programId);
			glUniform3iv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetIVec4(const string& name, const glm::ivec4* values, unsigned int nValues)
		{
			glUseProgram(_impl.programId);
			glUniform4iv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, &values[0][0]);
		}

		void RenderShaderProgram::SetMat2(const string& name, const glm::mat2* value, unsigned int nValues)
		{
			glUseProgram(_impl.programId);
			glUniformMatrix2fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, GL_FALSE, &value[0][0][0]);
		}

		void RenderShaderProgram::SetMat3(const string& name, const glm::mat3* value, unsigned int nValues)
		{
			glUseProgram(_impl.programId);
			glUniformMatrix3fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, GL_FALSE, &value[0][0][0]);
		}

		void RenderShaderProgram::SetMat4(const string& name, const glm::mat4* value, unsigned int nValues)
		{
			glUseProgram(_impl.programId);
			glUniformMatrix4fv(glGetUniformLocation(_impl.programId, name.c_str()), nValues, GL_FALSE, &value[0][0][0]);
		}

		RenderShaderProgram::RenderShaderProgram(
			const RenderVertexShader& vertexShader,
			const RenderFragmentShader& fragmentShader
		)
			:
			_impl(Impl::MakeImpl(vertexShader, fragmentShader)) {}

		RenderShaderProgram::RenderShaderProgram(
			const RenderVertexShader& vertexShader,
			const RenderGeometryShader& geometryShader,
			const RenderFragmentShader& fragmentShader
		)
			:
			_impl(Impl::MakeImpl(vertexShader, geometryShader, fragmentShader)) {}

		RenderShaderProgram::~RenderShaderProgram()
		{
			delete& _impl;
		}

		void RenderShaderProgram::Bind()
		{
			// TODO: Can optimize by saving state and only making gl calls
			// when necessary
			glUseProgram(_impl.programId);

			glDepthMask(_impl.depthWrite ? GL_TRUE : GL_FALSE);

			if (_impl.backFaceCullEnabled)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}

			switch (_impl.depthFunction)
			{
			case DepthFunction::NEVER:
				glDepthFunc(GL_NEVER);
				break;
			case DepthFunction::LESS_THAN:
				glDepthFunc(GL_LESS);
				break;
			case DepthFunction::LESS_THAN_OR_EQUAL:
				glDepthFunc(GL_LEQUAL);
				break;
			case DepthFunction::EQUAL:
				glDepthFunc(GL_EQUAL);
				break;
			case DepthFunction::NOT_EQUAL:
				glDepthFunc(GL_NOTEQUAL);
				break;
			case DepthFunction::GREATER_THAN:
				glDepthFunc(GL_GREATER);
				break;
			case DepthFunction::GREATER_THAN_OR_EQUAL:
				glDepthFunc(GL_GEQUAL);
				break;
			case DepthFunction::ALWAYS:
				glDepthFunc(GL_ALWAYS);
				break;
			}
		}
	} // namespace RenderingModule
} // namespace PRE