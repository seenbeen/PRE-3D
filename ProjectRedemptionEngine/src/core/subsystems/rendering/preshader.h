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
		using PRE::RenderingModule::RenderShaderProgram;

		class PREShader
		{
			PREShader& operator=(const PREShader&) = delete;
			PREShader(const PREShader&) = delete;

			friend class PRERendering;
			friend class PREMaterial;

		public:
			void SetBool(const string& name, bool value);
			void SetInt(const string& name, int value);
			void SetFloat(const string& name, float value);

			void SetVec2(const string& name, const glm::vec2& value);
			void SetVec3(const string& name, const glm::vec3& value);
			void SetVec4(const string& name, const glm::vec4& value);

			void SetMat2(const string& name, const glm::mat2& value);
			void SetMat3(const string& name, const glm::mat3& value);
			void SetMat4(const string& name, const glm::mat4& value);

		private:
			RenderShaderProgram& _shaderProgram;

			PREShader(RenderShaderProgram& shaderProgram);
			~PREShader();
		};
	} // namespace Core
} // namespace PRE