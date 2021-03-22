#pragma once
#include <string>

#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderShaderProgram;

		using std::string;

		class RenderGeometryShader
		{
			RenderGeometryShader& operator=(const RenderGeometryShader&) = delete;
			RenderGeometryShader(const RenderGeometryShader&) = delete;

			friend class Renderer;
			friend class RenderShaderProgram;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderGeometryShader;
				friend class RenderShaderProgram;

			private:
				const GLuint shaderId;

				static Impl& MakeImpl(const string& shaderSource);

				Impl(GLuint shaderId);
				~Impl();
			};

		private:
			Impl& _impl;

			RenderGeometryShader(const string& shaderSource);
			~RenderGeometryShader();
		};
	} // namespace RenderingModule
} // namespace PRE