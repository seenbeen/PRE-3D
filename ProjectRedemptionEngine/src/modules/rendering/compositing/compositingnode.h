#pragma once

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class CompositingTarget;

		class CompositingNode
		{
		protected:
			CompositingNode();
			~CompositingNode();

			virtual void OnInitialize() = 0;
			virtual void OnRender() = 0;
			virtual void OnShutdown() = 0;
		};
	} // namespace RenderingModule
} // namespace PRE