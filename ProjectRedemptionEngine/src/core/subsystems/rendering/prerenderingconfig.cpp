#include <core/subsystems/rendering/prerenderingconfig.h>

#include <string>

namespace PRE
{
	namespace Core
	{
		PRERenderingConfig::PRERenderingConfig(
			const string& windowTitle,
			unsigned int windowWidth,
			unsigned int windowHeight
		)
			:
			windowTitle(windowTitle),
			windowWidth(windowWidth),
			windowHeight(windowHeight) {}

	} // namespace Core
} // namespace PRE