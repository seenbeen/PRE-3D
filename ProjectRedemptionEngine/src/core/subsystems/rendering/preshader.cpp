#include <core/subsystems/rendering/preshader.h>

#include <string>
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		using std::string;
		using PRE::RenderingModule::RenderShaderProgram;

		void PREShader::SetBool(const string& name, bool value)
		{
			_shaderProgram.SetBool(name, value);
		}

		void PREShader::SetInt(const string& name, int value)
		{
			_shaderProgram.SetInt(name, value);
		}

		void PREShader::SetFloat(const string& name, float value)
		{
			_shaderProgram.SetFloat(name, value);
		}

		void PREShader::SetVec2(const string& name, const glm::vec2& value)
		{
			_shaderProgram.SetVec2(name, value);
		}

		void PREShader::SetVec3(const string& name, const glm::vec3& value)
		{
			_shaderProgram.SetVec3(name, value);
		}

		void PREShader::SetVec4(const string& name, const glm::vec4& value)
		{
			_shaderProgram.SetVec4(name, value);
		}

		void PREShader::SetMat2(const string& name, const glm::mat2& value)
		{
			_shaderProgram.SetMat2(name, value);
		}

		void PREShader::SetMat3(const string& name, const glm::mat3& value)
		{
			_shaderProgram.SetMat3(name, value);
		}

		void PREShader::SetMat4(const string& name, const glm::mat4& value)
		{
			_shaderProgram.SetMat4(name, value);
		}

		PREShader::PREShader(RenderShaderProgram& shaderProgram)
			:
			_shaderProgram(shaderProgram) {}

		PREShader::~PREShader() {}
	} // namespace Core
} // namespace PRE