#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <core/preapplication.h>

#include <core/preapplicationconfig.h>
#include <core/preapplicationcontext.h>

// #include <core/subsystems/physics/prephysics.h>
#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/time/pretime.h>
#include <core/subsystems/world/preworld.h>

namespace PRE
{
	namespace Core
	{
		void PREApplication::Run(const PREApplicationConfig& applicationConfig)
		{
			auto application = new PREApplication(applicationConfig);

			application->Run();

			delete application;
		}
		
		PREApplication::PREApplication(
			const PREApplicationConfig& applicationConfig
		) : _applicationContext(applicationConfig, *this)
		{
			_isRunning = false;
		}

		PREApplication::~PREApplication() {}

		void PREApplication::Run()
		{
			_applicationContext.Initialize();

			_isRunning = true;
#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop_arg(
				[](void* pVoidApplication)
				{
					auto pApplication = static_cast<PREApplication*>(pVoidApplication);
					if (!pApplication->_isRunning) {
						pApplication->_applicationContext.Shutdown();
						emscripten_cancel_main_loop();
						return;
					}
					pApplication->_applicationContext.Update();
				},
				static_cast<void*>(this),
				0,
				true
			);
#else
			while (_isRunning)
			{
				_applicationContext.Update();
			}
			_applicationContext.Shutdown();
#endif
		}

		void PREApplication::Quit()
		{
			_isRunning = false;
		}
	}
}