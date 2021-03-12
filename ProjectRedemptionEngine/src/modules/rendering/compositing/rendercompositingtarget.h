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

				RenderTexture& target;

				static Impl& MakeImpl(unsigned int width, unsigned int height);
				static Impl& MakeImpl(
					unsigned int rightWidth, unsigned int rightHeight,
					unsigned int leftWidth, unsigned int leftHeight,
					unsigned int topWidth, unsigned int topHeight,
					unsigned int bottomWidth, unsigned int bottomHeight,
					unsigned int frontWidth, unsigned int frontHeight,
					unsigned int backWidth, unsigned int backHeight
				);

				Impl(
					GLuint gBufferId,
					RenderTexture& target
				);

				~Impl();
			};

		private:
			Impl& _impl;
			
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