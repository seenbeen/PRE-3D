#pragma once
#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderTexture;

		class CompositingTarget
		{
			CompositingTarget& operator=(const CompositingTarget&) = delete;
			CompositingTarget(const CompositingTarget&) = delete;

			friend class Renderer;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class CompositingTarget;

			private:
				const GLuint gBufferId;

				RenderTexture& position;
				RenderTexture& normals;
				RenderTexture& albedoSpecular;

				static Impl& MakeImpl(unsigned int width, unsigned int height);

				Impl(
					GLuint gBufferId,
					RenderTexture& position,
					RenderTexture& normals,
					RenderTexture& albedoSpecular
				);
				~Impl();
			};

		private:
			Impl& _impl;
			
			CompositingTarget(unsigned int width, unsigned int height);
			~CompositingTarget();

			RenderTexture& GetPosition();
			RenderTexture& GetNormals();
			RenderTexture& GetAlbedoSpecular();

			static void Bind(CompositingTarget* pCompositingTarget);
		};
	} // namespace RenderingModule
} // namespace PRE