#include <core/subsystems/rendering/preshader.h>

#include <string>
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		using std::string;
		using PRE::RenderingModule::RenderShaderProgram;

		const string PREShader::IS_FIRST_LIGHT_PASS = "PRE_IS_FIRST_LIGHT_PASS";
		const string PREShader::LIGHT_ACCUMULATOR_SAMPLER_SIZE = "PRE_LIGHT_ACCUMULATOR_SAMPLER_SIZE";
		const string PREShader::LIGHT_ACCUMULATOR_SAMPLER = "PRE_LIGHT_ACCUMULATOR_SAMPLER";
		const int PREShader::LIGHT_ACCUMULATOR_BINDING = 0;

		const string PREShader::SHADOW_CUBE_MAP_MATRICES = "PRE_SHADOW_CUBE_MAP_MATRICES";
		const string PREShader::SHADOW_MAP_SAMPLER = "PRE_SHADOW_MAP_SAMPLER";
		const string PREShader::SHADOW_CUBE_MAP_SAMPLER = "PRE_SHADOW_CUBE_MAP_SAMPLER";
		const int PREShader::SHADOW_MAP_BINDING = 1;
		const int PREShader::SHADOW_CUBE_MAP_BINDING = 2;
		const string PREShader::SHADOW_MAP_VIEW_MATRIX = "PRE_SHADOW_MAP_VIEW_MATRIX";
		const string PREShader::SHADOW_MAP_PROJECTION_MATRIX = "PRE_SHADOW_MAP_PROJECTION_MATRIX";
		const string PREShader::SHADOW_MAP_FAR_CLIPPING_PLANE = "PRE_SHADOW_MAP_FAR_CLIPPING_PLANE";

		const string PREShader::AMBIENT_LIGHT_FLAG = "PRE_AMBIENT_LIGHT_FLAG";
		const string PREShader::POINT_LIGHT_FLAG = "PRE_POINT_LIGHT_FLAG";
		const string PREShader::SPOT_LIGHT_FLAG = "PRE_SPOT_LIGHT_FLAG";
		const string PREShader::DIRECTIONAL_LIGHT_FLAG = "PRE_DIRECTIONAL_LIGHT_FLAG";

		const string PREShader::VIEW_POSITION = "PRE_VIEW_POSITION";

		const string PREShader::LIGHT_POSITION = "PRE_LIGHT_POSITION";
		const string PREShader::LIGHT_DIRECTION = "PRE_LIGHT_DIRECTION";
		const string PREShader::LIGHT_COLOR = "PRE_LIGHT_COLOR";
		const string PREShader::LIGHT_ATTENUATION_LINEAR = "PRE_LIGHT_ATTENUATION_LINEAR";
		const string PREShader::LIGHT_ATTENUATION_QUADRATIC = "PRE_LIGHT_ATTENUATION_QUADRATIC";
		const string PREShader::LIGHT_INNER_RADIUS = "PRE_LIGHT_INNER_RADIUS";
		const string PREShader::LIGHT_OUTER_RADIUS = "PRE_LIGHT_OUTER_RADIUS";


		void PREShader::SetBackFaceCulling(bool enabled)
		{
			_shaderProgram.SetBackFaceCulling(enabled);
		}

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

		void PREShader::SetIVec2(const string& name, const glm::ivec2& value)
		{
			_shaderProgram.SetIVec2(name, value);
		}

		void PREShader::SetIVec3(const string& name, const glm::ivec3& value)
		{
			_shaderProgram.SetIVec3(name, value);
		}

		void PREShader::SetIVec4(const string& name, const glm::ivec4& value)
		{
			_shaderProgram.SetIVec4(name, value);
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

		void PREShader::SetVec2(const string& name, const glm::vec2* values, unsigned int nValues)
		{
			_shaderProgram.SetVec2(name, values, nValues);
		}

		void PREShader::SetVec3(const string& name, const glm::vec3* values, unsigned int nValues)
		{
			_shaderProgram.SetVec3(name, values, nValues);
		}

		void PREShader::SetVec4(const string& name, const glm::vec4* values, unsigned int nValues)
		{
			_shaderProgram.SetVec4(name, values, nValues);
		}

		void PREShader::SetIVec2(const string& name, const glm::ivec2* values, unsigned int nValues)
		{
			_shaderProgram.SetIVec2(name, values, nValues);
		}

		void PREShader::SetIVec3(const string& name, const glm::ivec3* values, unsigned int nValues)
		{
			_shaderProgram.SetIVec3(name, values, nValues);
		}

		void PREShader::SetIVec4(const string& name, const glm::ivec4* values, unsigned int nValues)
		{
			_shaderProgram.SetIVec4(name, values, nValues);
		}

		void PREShader::SetMat2(const string& name, const glm::mat2* values, unsigned int nValues)
		{
			_shaderProgram.SetMat2(name, values, nValues);
		}

		void PREShader::SetMat3(const string& name, const glm::mat3* values, unsigned int nValues)
		{
			_shaderProgram.SetMat3(name, values, nValues);
		}

		void PREShader::SetMat4(const string& name, const glm::mat4* values, unsigned int nValues)
		{
			_shaderProgram.SetMat4(name, values, nValues);
		}

		PREShader::PREShader(
			RenderShaderProgram& shaderProgram,
			RenderShaderProgram& shadowShaderProgram,
			RenderShaderProgram& cubeMapShadowShaderProgram
		)
			:
			_shaderProgram(shaderProgram),
			_shadowShaderProgram(shadowShaderProgram),
			_cubeMapShadowShaderProgram(cubeMapShadowShaderProgram) {}

		PREShader::~PREShader() {}
	} // namespace Core
} // namespace PRE