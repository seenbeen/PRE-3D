#pragma once
#include <core/preapplicationconfig.h>
#include <core/preapplicationcontext.h>

namespace PRE
{
	namespace Core
	{
		// class PREPhysics;
		class PRERendering;
		class PRETime;
		class PREWorld;

		class PREApplication
		{
			friend class PREApplicationContext;

		public:
			static void Run(const PREApplicationConfig& applicationConfig);

		private:
			bool _isRunning;

			PREApplicationContext _applicationContext;

			PREApplication(const PREApplicationConfig& applicationConfig);

			~PREApplication();

			void Run();
			void Quit();
		};
	} // namespace Core
} // namespace PRE