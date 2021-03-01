#pragma once

namespace PRE
{
	namespace RenderingModule
	{
		class CompositingTarget;

		class Renderer;
		
		class RenderNode
		{
			RenderNode& operator=(const RenderNode&) = delete;
			RenderNode(const RenderNode&) = delete;

			friend class Renderer;

		private:
			CompositingTarget& compositingTarget;

			RenderNode();
			~RenderNode();

			
		};
	} // namespace RenderingModule
} // namespace PRE