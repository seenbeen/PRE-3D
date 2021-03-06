#pragma once
#include <core/preapplicationconfig.h>

namespace PRE
{
	namespace Core
	{
		class PREInput;
		class PRERendering;
		class PRETime;
		class PREWorld;
		class PREAssetManager;

		class PREApplication;

		class PREApplicationContext
		{
			PREApplicationContext& operator=(const PREApplicationContext&) = delete;
			PREApplicationContext(const PREApplicationContext&) = delete;

			friend class PREApplication;

		public:
			PREInput& input;
			PRERendering& rendering;
			PRETime& time;
			PREWorld& world;
			PREAssetManager& assetManager;

			void Quit();

		private:
			PREApplicationConfig::OnInitialize _onInitialize;
			PREApplicationConfig::OnShutdown _onShutdown;

			PREApplication& _application;

			PREApplicationContext(
				const PREApplicationConfig& applicationConfig,
				PREApplication& application
			);
			~PREApplicationContext();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE