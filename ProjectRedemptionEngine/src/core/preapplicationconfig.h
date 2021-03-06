#pragma once
#include <core/subsystems/input/preinputconfig.h>
#include <core/subsystems/rendering/prerenderingconfig.h>
#include <core/subsystems/time/pretimeconfig.h>
#include <core/subsystems/world/preworldconfig.h>
#include <core/subsystems/asset/preassetmanagerconfig.h>

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

			using OnInitialize = void(&)(PREApplicationContext&);
			using OnShutdown = void(&)(PREApplicationContext&);

			friend class PREApplicationContext;
			friend class PREApplication;

		public:
			PREApplicationConfig(
				const PREInputConfig& inputConfig,
				const PRERenderingConfig& renderingConfig,
				const PRETimeConfig& timeConfig,
				const PREWorldConfig& worldConfig,
				const PREAssetManagerConfig& assetManagerConfig,
				OnInitialize& onInitialize,
				OnShutdown& onShutdown
			);

		private:
			const PREInputConfig _inputConfig;
			const PRERenderingConfig _renderingConfig;
			const PRETimeConfig _timeConfig;
			const PREWorldConfig _worldConfig;
			const PREAssetManagerConfig _assetManagerConfig;

			OnInitialize& _onInitialize;
			OnShutdown& _onShutdown;
		};
	} // namespace Core
} // namespace PRE