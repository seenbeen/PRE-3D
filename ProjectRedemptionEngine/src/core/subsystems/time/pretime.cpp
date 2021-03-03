#include <core/subsystems/time/pretime.h>

#include <include/modules/time.h>

#include <core/subsystems/time/pretimeconfig.h>

namespace PRE
{
	namespace Core
	{
		PRETime& PRETime::MakePRETime(
			const PRETimeConfig& timeConfig,
			PREApplicationContext& applicationContext
		)
		{
			auto& time = *(new Time());
			return *(new PRETime(applicationContext, time));
		}

		float PRETime::GetDeltaTime()
		{
			return _time.GetDeltaTime();
		}

		PRETime::PRETime(PREApplicationContext& applicationContext, Time& time)
			:
			_applicationContext(applicationContext),
			_time(time) {}

		PRETime::~PRETime()
		{
			delete &_time;
		}

		void PRETime::Initialize() {}

		void PRETime::Update()
		{
			_time.Update();
		}

		void PRETime::Shutdown() {}
	}
}