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
		class RenderGeometryShader;
		class RenderFragmentShader;
		class RenderMaterial;

		using std::string;

		class RenderShaderProgram
		{
			RenderShaderProgram& operator=(const RenderShaderProgram&) = delete;
			RenderShaderProgram(const RenderShaderProgram&) = delete;

			friend class Renderer;
			friend class RenderMaterial;

		public:
			enum class DepthFunction {
				NEVER,
				LESS_THAN, LESS_THAN_OR_EQUAL,
				EQUAL, NOT_EQUAL,
				GREATER_THAN, GREATER_THAN_OR_EQUAL,
				ALWAYS
			};

		private:
			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderShaderProgram;

			private:
				const GLuint programId;

				bool backFaceCullEnabled;
				bool depthWrite;
				DepthFunction depthFunction;

				static Impl& MakeImpl(
					const RenderVertexShader& vertexShader,
					const RenderFragmentShader& fragmentShader
				);

				static Impl& MakeImpl(
					const RenderVertexShader& vertexShader,
					const RenderGeometryShader& geometryShader,
					const RenderFragmentShader& fragmentShader
				);

				Impl(GLuint programId);
				~Impl();
			};

		public:
			void SetBackFaceCulling(bool enabled);
			void SetDepthWrite(bool enabled);
			void SetDepthFunction(const DepthFunction& depthFunction);

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
			Impl& _impl;

			RenderShaderProgram(
				const RenderVertexShader& vertexShader,
				const RenderFragmentShader& fragmentShader
			);

			RenderShaderProgram(
				const RenderVertexShader& vertexShader,
				const RenderGeometryShader& geometryShader,
				const RenderFragmentShader& fragmentShader
			);

			~RenderShaderProgram();

			void Bind();
		};
	} // namespace RenderingModule
} // namespace PRE