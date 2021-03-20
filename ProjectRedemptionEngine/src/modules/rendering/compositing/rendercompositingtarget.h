#pragma once
#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderTexture;
		class RenderMaterial;

		class RenderCompositingTarget
		{
			RenderCompositingTarget& operator=(const RenderCompositingTarget&) = delete;
			RenderCompositingTarget(const RenderCompositingTarget&) = delete;

			friend class Renderer;
			friend class RenderMaterial;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderCompositingTarget;

			private:
				const unsigned int width;
				const unsigned int height;

				const GLuint bufferId;
				const GLuint* const pRenderBufferId;

				RenderTexture& target;

				static Impl& MakeImpl(
					unsigned int size, bool isCubeMap
				);

				static Impl& MakeImpl(
					unsigned int width, unsigned int height
				);
				static Impl& MakeImpl(
					unsigned int rightWidth, unsigned int rightHeight,
					unsigned int leftWidth, unsigned int leftHeight,
					unsigned int topWidth, unsigned int topHeight,
					unsigned int bottomWidth, unsigned int bottomHeight,
					unsigned int frontWidth, unsigned int frontHeight,
					unsigned int backWidth, unsigned int backHeight
				);

				Impl(
					unsigned int width,
					unsigned int height,
					GLuint bufferId,
					const GLuint* pRenderbufferId,
					RenderTexture& target
				);

				~Impl();
			};

		private:
			Impl& _impl;

			// constructs a depth-map compositing target for shadowmaps
			RenderCompositingTarget(unsigned int size, bool isCubeMap);

			RenderCompositingTarget(unsigned int width, unsigned int height);
			RenderCompositingTarget(
				unsigned int rightWidth, unsigned int rightHeight,
				unsigned int leftWidth, unsigned int leftHeight,
				unsigned int topWidth, unsigned int topHeight,
				unsigned int bottomWidth, unsigned int bottomHeight,
				unsigned int frontWidth, unsigned int frontHeight,
				unsigned int backWidth, unsigned int backHeight
			);
			~RenderCompositingTarget();

			RenderTexture& GetTarget();

			static void Bind(RenderCompositingTarget* pCompositingTarget);
		};
	} // namespace RenderingModule
} // namespace PRE