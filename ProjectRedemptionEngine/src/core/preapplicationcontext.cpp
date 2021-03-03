#include <core/preapplicationcontext.h>

#include <core/preapplicationconfig.h>
#include <core/preapplication.h>

#include <core/preapplicationconfig.h>

#include <core/subsystems/input/preinput.h>
#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/time/pretime.h>
#include <core/subsystems/world/preworld.h>

namespace PRE
{
	namespace Core
	{
		void PREApplicationContext::Quit()
		{
			_application.Quit();
		}

		PREApplicationContext::PREApplicationContext(
			const PREApplicationConfig& applicationConfig,
			PREApplication& application
		) :
			input(
				PREInput::MakePREInput(
					applicationConfig._inputConfig,
					*this
				)
			),
			rendering(
				PRERendering::MakePRERendering(
					applicationConfig._renderingConfig,
					*this
				)
			),
			time(
				PRETime::MakePRETime(
					applicationConfig._timeConfig,
					*this
				)
			),
			world(
				PREWorld::MakePREWorld(
					applicationConfig._worldConfig,
					*this
				)
			),
			_application(application),
			_onInitialize(applicationConfig._onInitialize),
			_onShutdown(applicationConfig._onShutdown) {}

		PREApplicationContext::~PREApplicationContext()
		{
			delete &world;
			delete &time;
			delete &rendering;
			delete &input;
		}

		void PREApplicationContext::Initialize()
		{
			input.Initialize();
			time.Initialize();
			world.Initialize();
			rendering.Initialize();

			_onInitialize(*this);
		}

		void PREApplicationContext::Update()
		{
			input.Update();
			time.Update();
			world.Update();
			rendering.Update();
		}

		void PREApplicationContext::Shutdown()
		{			
			_onShutdown(*this);

			rendering.Shutdown();
			world.Shutdown();
			time.Shutdown();
			input.Shutdown();
		}
	}
}