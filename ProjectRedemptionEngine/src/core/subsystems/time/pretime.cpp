#include <chrono>
#include <thread>

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

		void PRETime::SetFrameLimit(unsigned int frameLimit)
		{
			_frameLimit = frameLimit;
		}

		unsigned int PRETime::GetFrameLimit()
		{
			return _frameLimit;
		}

		float PRETime::GetDeltaTime()
		{
			return _time.GetDeltaTime();
		}

		const float PRETime::MILLIS_IN_SECOND = 1000;

		PRETime::PRETime(PREApplicationContext& applicationContext, Time& time)
			:
			_applicationContext(applicationContext),
			_time(time),
			_frameLimit(0) {}

		PRETime::~PRETime()
		{
			delete &_time;
		}

		void PRETime::Initialize() {}

		void PRETime::Update()
		{
			_time.Update();
			if (_frameLimit != 0)
			{
				auto secondsPerFrame = 1.0f / _frameLimit;
				std::this_thread::sleep_for(
					std::chrono::milliseconds((long)(
						(secondsPerFrame - _time.GetDeltaTime()) * MILLIS_IN_SECOND)
					)
				);
			}
		}

		void PRETime::Shutdown() {}
	}
}