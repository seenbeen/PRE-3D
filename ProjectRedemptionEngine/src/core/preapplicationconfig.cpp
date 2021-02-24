#include <core/preapplicationconfig.h>

namespace PRE
{
	namespace Core
	{
		PREApplicationConfig::PREApplicationConfig(
			int options,
			OnInitialize& onInitialize,
			OnShutdown& onShutdown
		) : _options(options), _onInitialize(onInitialize), _onShutdown(onShutdown) {}
	}
}