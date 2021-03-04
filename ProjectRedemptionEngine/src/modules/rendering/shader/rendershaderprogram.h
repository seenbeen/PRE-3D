#pragma once
#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderVertexShader;
		class RenderFragmentShader;
		class RenderMaterial;

		using std::string;

		class RenderShaderProgram
		{
			RenderShaderProgram& operator=(const RenderShaderProgram&) = delete;
			RenderShaderProgram(const RenderShaderProgram&) = delete;

			friend class Renderer;
			friend class RenderMaterial;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderShaderProgram;

			private:
				const GLuint programId;

				static Impl& MakeImpl(
					const RenderVertexShader& vertexShader,
					const RenderFragmentShader& fragmentShader
				);
				Impl(GLuint programId);
				~Impl();
			};

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
			Impl& _impl;

			RenderShaderProgram(
				const RenderVertexShader& vertexShader,
				const RenderFragmentShader& fragmentShader
			);
			~RenderShaderProgram();

			void Bind();
		};
	} // namespace RenderingModule
} // namespace PRE