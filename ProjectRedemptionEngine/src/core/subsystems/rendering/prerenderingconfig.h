#pragma once
#include <string>

namespace PRE
{
	namespace Core
	{
		using std::string;

		struct PRERenderingConfig
		{
			const string windowTitle;
			const unsigned int windowWidth;
			const unsigned int windowHeight;

			PRERenderingConfig(
				const string& windowTitle,
				unsigned int windowWidth,
				unsigned int windowHeight
			);
		};
	} // namespace Core
} // namespace PRE