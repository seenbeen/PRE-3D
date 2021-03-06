#pragma once
#include <string>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PREMaterial;

		using std::string;
		using PRE::RenderingModule::RenderVertexShader;
		using PRE::RenderingModule::RenderGeometryShader;
		using PRE::RenderingModule::RenderFragmentShader;
		using PRE::RenderingModule::RenderShaderProgram;

		class PREShader
		{
			PREShader& operator=(const PREShader&) = delete;
			PREShader(const PREShader&) = delete;

			friend class PRERendering;
			friend class PREMaterial;

		public:
			// this is the accumulated light buffer from previous light passes
			static const string IS_FIRST_LIGHT_PASS;
			static const string LIGHT_ACCUMULATOR_SAMPLER_SIZE;
			static const string LIGHT_ACCUMULATOR_SAMPLER;
			static const int LIGHT_ACCUMULATOR_BINDING;

			static const string SHADOW_CUBE_MAP_MATRICES;
			static const string SHADOW_MAP_SAMPLER;
			static const string SHADOW_CUBE_MAP_SAMPLER;
			static const int SHADOW_MAP_BINDING;
			static const int SHADOW_CUBE_MAP_BINDING;
			static const string SHADOW_MAP_VIEW_MATRIX; // light's POV
			static const string SHADOW_MAP_PROJECTION_MATRIX;
			static const string SHADOW_MAP_FAR_CLIPPING_PLANE;

			// mutually exclusive, to be used as part of lighting equation
			static const string AMBIENT_LIGHT_FLAG;
			static const string POINT_LIGHT_FLAG;
			static const string SPOT_LIGHT_FLAG;
			static const string DIRECTIONAL_LIGHT_FLAG;

			static const string VIEW_POSITION;

			static const string LIGHT_POSITION;
			static const string LIGHT_DIRECTION; // spot lights + directional lights
			static const string LIGHT_COLOR;
			static const string LIGHT_ATTENUATION_LINEAR;
			static const string LIGHT_ATTENUATION_QUADRATIC;
			static const string LIGHT_INNER_RADIUS; // spot lights
			static const string LIGHT_OUTER_RADIUS; // spot lights

			void SetBackFaceCulling(bool enabled);

			void SetBool(const string& name, bool value);
			void SetInt(const string& name, int value);
			void SetFloat(const string& name, float value);

			void SetVec2(const string& name, const glm::vec2& value);
			void SetVec3(const string& name, const glm::vec3& value);
			void SetVec4(const string& name, const glm::vec4& value);

			void SetIVec2(const string& name, const glm::ivec2& value);
			void SetIVec3(const string& name, const glm::ivec3& value);
			void SetIVec4(const string& name, const glm::ivec4& value);

			void SetMat2(const string& name, const glm::mat2& value);
			void SetMat3(const string& name, const glm::mat3& value);
			void SetMat4(const string& name, const glm::mat4& value);

			void SetVec2(const string& name, const glm::vec2* values, unsigned int nValues);
			void SetVec3(const string& name, const glm::vec3* values, unsigned int nValues);
			void SetVec4(const string& name, const glm::vec4* values, unsigned int nValues);

			void SetIVec2(const string& name, const glm::ivec2* values, unsigned int nValues);
			void SetIVec3(const string& name, const glm::ivec3* values, unsigned int nValues);
			void SetIVec4(const string& name, const glm::ivec4* values, unsigned int nValues);

			void SetMat2(const string& name, const glm::mat2* values, unsigned int nValues);
			void SetMat3(const string& name, const glm::mat3* values, unsigned int nValues);
			void SetMat4(const string& name, const glm::mat4* values, unsigned int nValues);

		private:
			RenderShaderProgram& _shaderProgram;
			RenderShaderProgram& _shadowShaderProgram;
			RenderShaderProgram& _cubeMapShadowShaderProgram;

			PREShader(
				RenderShaderProgram& shaderProgram,
				RenderShaderProgram& shadowShaderProgram,
				RenderShaderProgram& cubeMapShadowShaderProgram
			);

			~PREShader();
		};
	} // namespace Core
} // namespace PRE