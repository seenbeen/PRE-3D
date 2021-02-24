#pragma once

namespace PRE
{
	namespace Core
	{
		class PREApplicationContext;

		class PRERendering
		{
			PRERendering& operator=(const PRERendering&) = delete;
			PRERendering(const PRERendering&) = delete;

			friend class PREApplicationContext;

		private:
			PRERendering();
			~PRERendering();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE