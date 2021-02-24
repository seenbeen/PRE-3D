#pragma once
#include <core/preapplicationconfig.h>

namespace PRE
{
	namespace Core
	{
		class PREApplication;

		// class PREPhysics;
		class PRERendering;
		class PRETime;
		class PREWorld;

		class PREApplicationContext
		{
			PREApplicationContext& operator=(const PREApplicationContext&) = delete;
			PREApplicationContext(const PREApplicationContext&) = delete;

			friend class PREApplication;

		public:
			// const PREPhysics* physics;
			PRERendering* const rendering;
			PRETime* const time;
			PREWorld* const world;
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