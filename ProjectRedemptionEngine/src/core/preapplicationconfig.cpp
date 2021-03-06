#include <core/preapplicationconfig.h>

#include <core/subsystems/input/preinputconfig.h>
#include <core/subsystems/rendering/prerenderingconfig.h>
#include <core/subsystems/time/pretimeconfig.h>
#include <core/subsystems/world/preworldconfig.h>
#include <core/subsystems/asset/preassetmanagerconfig.h>

namespace PRE
{
	namespace Core
	{
		PREApplicationConfig::PREApplicationConfig(
			const PREInputConfig& inputConfig,
			const PRERenderingConfig& renderingConfig,
			const PRETimeConfig& timeConfig,
			const PREWorldConfig& worldConfig,
			const PREAssetManagerConfig& assetManagerConfig,
			OnInitialize& onInitialize,
			OnShutdown& onShutdown
		)
			:
			_inputConfig(inputConfig),
			_renderingConfig(renderingConfig),
			_timeConfig(timeConfig),
			_worldConfig(worldConfig),
			_assetManagerConfig(assetManagerConfig),
			_onInitialize(onInitialize),
			_onShutdown(onShutdown) {}
	}
}