#pragma once

namespace PRE
{
	namespace Core
	{
		class PREApplicationContext;
		class PREApplication;
		
		class PREApplicationConfig
		{
			PREApplicationConfig& operator=(const PREApplicationConfig&) = delete;
			PREApplicationConfig(const PREApplicationConfig&) = delete;

			using OnInitialize = void(&)(PREApplicationContext*);
			using OnShutdown = void(&)(PREApplicationContext*);

			friend class PREApplicationContext;
			friend class PREApplication;

		public:
			enum class Options {
				//USE_PHYSICS = 1 << 0,
				USE_RENDERING = 1 << 1,
				USE_TIME = 1 << 2
			};

			PREApplicationConfig(int options, OnInitialize& onInitialize, OnShutdown& onShutdown);

		private:
			const int _options;
			OnInitialize& _onInitialize;
			OnShutdown& _onShutdown;
		};
	} // namespace Core
} // namespace PRE