#include <core/subsystems/asset/preassetmanagerconfig.h>

#include <string>

namespace PRE
{
	namespace Core
	{
		using std::string;

		PREAssetManagerConfig::PREAssetManagerConfig(const string& rootAssetPath)
			:
			rootAssetPath(rootAssetPath) {}
	} // namespace Core
} // namespace PRE