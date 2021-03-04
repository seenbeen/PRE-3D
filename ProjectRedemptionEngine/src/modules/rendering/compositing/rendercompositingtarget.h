#pragma once
#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderCompositingNode;
		class RenderTexture;

		class RenderCompositingTarget
		{
			RenderCompositingTarget& operator=(const RenderCompositingTarget&) = delete;
			RenderCompositingTarget(const RenderCompositingTarget&) = delete;

			friend class Renderer;
			friend class RenderCompositingNode;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderCompositingTarget;

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
			
			RenderCompositingTarget(unsigned int width, unsigned int height);
			~RenderCompositingTarget();

			RenderTexture& GetPosition();
			RenderTexture& GetNormals();
			RenderTexture& GetAlbedoSpecular();

			static void Bind(RenderCompositingTarget* pCompositingTarget);
		};
	} // namespace RenderingModule
} // namespace PRE