#include <core/preapplicationcontext.h>

#include <core/preapplicationconfig.h>
#include <core/preapplication.h>

#include <core/preapplicationconfig.h>

#include <core/subsystems/input/preinput.h>
#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/time/pretime.h>
#include <core/subsystems/world/preworld.h>
#include <core/subsystems/asset/preassetmanager.h>

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
			assetManager(
				PREAssetManager::MakePREAssetManager(
					applicationConfig._assetManagerConfig,
					*this
				)
			),
			_application(application),
			_onInitialize(applicationConfig._onInitialize),
			_onShutdown(applicationConfig._onShutdown) {}

		PREApplicationContext::~PREApplicationContext()
		{
			delete& time;
			delete& input;
			delete& world;
			delete& rendering;
			delete& assetManager;
		}

		void PREApplicationContext::Initialize()
		{
			time.Initialize();
			input.Initialize();
			rendering.Initialize();
			world.Initialize();
			assetManager.Initialize();

			_onInitialize(*this);
		}

		void PREApplicationContext::Update()
		{
			time.Update();
			input.Update();
			world.Update();
			rendering.Update();
			assetManager.Update();
		}

		void PREApplicationContext::Shutdown()
		{			
			_onShutdown(*this);

			time.Shutdown();
			input.Shutdown();
			world.Shutdown();
			rendering.Shutdown();
			assetManager.Shutdown();
		}
	}
}