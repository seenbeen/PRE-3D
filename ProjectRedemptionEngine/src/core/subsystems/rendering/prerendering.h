#pragma once
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PREApplicationContext;

		using PRE::RenderingModule::Renderer;

		class PRERendering
		{
			PRERendering& operator=(const PRERendering&) = delete;
			PRERendering(const PRERendering&) = delete;

			friend class PREApplicationContext;

		private:
			Renderer& _renderer;

			PRERendering();
			~PRERendering();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE