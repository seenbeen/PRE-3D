#include <core/preapplicationcontext.h>

#include <core/preapplicationconfig.h>
#include <core/preapplication.h>

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
				applicationConfig._options & (int)PREApplicationConfig::Options::USE_RENDERING
				? new PREInput()
				: nullptr
			),
			// applicationConfig._options & (int)PREApplicationConfig::Options::USE_PHYSICS
			// ? new PREPhysics()
			// : nullptr,
			rendering(
				applicationConfig._options & (int)PREApplicationConfig::Options::USE_RENDERING
				? new PRERendering(*this)
				: nullptr
			),
			time(
				applicationConfig._options& (int)PREApplicationConfig::Options::USE_TIME
				? new PRETime()
				: nullptr
			),
			world(new PREWorld(*this)),
			_application(application),
			_onInitialize(applicationConfig._onInitialize),
			_onShutdown(applicationConfig._onShutdown) {}

		PREApplicationContext::~PREApplicationContext()
		{
			delete world;
			delete time;
			delete rendering;
			delete input;
		}

		void PREApplicationContext::Initialize()
		{
			if (input != nullptr)
			{
				input->Initialize();
			}

			if (time != nullptr)
			{
				time->Initialize();
			}

			world->Initialize();

			if (rendering != nullptr)
			{
				rendering->Initialize();
			}

			_onInitialize(this);
		}

		void PREApplicationContext::Update()
		{
			if (input != nullptr)
			{
				input->Update();
			}

			if (time != nullptr)
			{
				time->Update();
			}

			world->Update();

			if (rendering != nullptr)
			{
				rendering->Update();
			}
		}

		void PREApplicationContext::Shutdown()
		{			
			_onShutdown(this);
			
			if (rendering != nullptr)
			{
				rendering->Shutdown();
			}
			
			world->Shutdown();

			if (time != nullptr)
			{
				time->Shutdown();
			}

			if (input != nullptr)
			{
				input->Shutdown();
			}
		}
	}
}