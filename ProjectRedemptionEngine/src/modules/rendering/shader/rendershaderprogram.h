#pragma once
#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class RenderMaterial;
		class Renderer;

		class RenderVertexShader;
		class RenderFragmentShader;

		using std::string;

		class RenderShaderProgram
		{
			RenderShaderProgram& operator=(const RenderShaderProgram&) = delete;
			RenderShaderProgram(const RenderShaderProgram&) = delete;

			friend class RenderMaterial;
			friend class Renderer;

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

		private:
			Impl& _impl;

			RenderShaderProgram(
				const RenderVertexShader& vertexShader,
				const RenderFragmentShader& fragmentShader
			);
			~RenderShaderProgram();

			void SetBool(const string& name, bool value) const;
			void SetInt(const string& name, int value) const;
			void SetFloat(const string& name, float value) const;

			void SetVec2(const string& name, const glm::vec2& value) const;
			void SetVec3(const string& name, const glm::vec3& value) const;
			void SetVec4(const string& name, const glm::vec4& value) const;

			void SetMat2(const string& name, const glm::mat2& value) const;
			void SetMat3(const string& name, const glm::mat3& value) const;
			void SetMat4(const string& name, const glm::mat4& value) const;

			void Bind() const;
		};
	} // namespace RenderingModule
} // namespace PRE