#pragma once
#include <string>

namespace PRE
{
	namespace Core
	{
		using std::string;

		struct PREAssetManagerConfig
		{
			const string rootAssetPath;

			PREAssetManagerConfig(const string& rootAssetPath);
		};
	} // namespace Core
} // namespace PRE