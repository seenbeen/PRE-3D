#pragma once

namespace PRE
{
	namespace RenderingModule
	{
		class CompositingNode;
		class RenderTexture;
		class Camera;
		class Renderable;

		class Renderer
		{
			Renderer& operator=(const Renderer&) = delete;
			Renderer(const Renderer&) = delete;

			friend class Camera;

		public:
			Renderer();
			~Renderer();

			void Initialize(); // validate and throw if there's a problem
			void Update();
			void Shutdown();

		private:
			void Render(RenderTexture& renderTexture);

		};
	} // namespace RenderingModule
} // namespace PRE