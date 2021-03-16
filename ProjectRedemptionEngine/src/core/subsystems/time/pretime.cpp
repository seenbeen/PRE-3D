#include <core/subsystems/time/pretime.h>

#include <algorithm>
#include <chrono>
#include <thread>

#include <include/modules/time.h>

#include <core/subsystems/time/pretimeconfig.h>

namespace PRE
{
	namespace Core
	{
		using std::chrono::nanoseconds;

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
			if (_frameLimit != 0)
			{
				_secondsPerFrame = 1.0l / _frameLimit;
			}
		}

		unsigned int PRETime::GetFrameLimit()
		{
			return _frameLimit;
		}

		float PRETime::GetDeltaTime()
		{
			return _deltaTime;
		}

		const long long PRETime::NANOS_IN_SECOND = 1000000000l;
		const int PRETime::FRAME_DELAY_SAMPLES = 120;

		PRETime::PRETime(PREApplicationContext& applicationContext, Time& time)
			:
			_deltaTime(0),
			_applicationContext(applicationContext),
			_time(time),
			_frameLimit(0),
			_secondsPerFrame(0),
			_priorFramesTimesSum(0) {}

		PRETime::~PRETime()
		{
			delete &_time;
		}

		void PRETime::Initialize() {}

		void PRETime::Update()
		{
			_time.Update();
			auto frameTime = _time.GetDeltaTime();
			if (_frameLimit != 0)
			{
				_priorFramesTimesSum += frameTime;
				_priorFramesTimes.push_back(frameTime);
				if (_priorFramesTimes.size() > FRAME_DELAY_SAMPLES)
				{
					_priorFramesTimesSum -= _priorFramesTimes.front();
					_priorFramesTimes.pop_front();
				}
				auto waitTimeSeconds = _secondsPerFrame - _priorFramesTimesSum / _priorFramesTimes.size();
				std::this_thread::sleep_for(
					std::chrono::nanoseconds((long long)(
						std::max(0.0l , waitTimeSeconds) * NANOS_IN_SECOND)
					)
				);
			}

			_time.Update();
			auto sleepTime = _time.GetDeltaTime();

			_deltaTime = (float)(frameTime + sleepTime);
		}

		void PRETime::Shutdown() {}
	}
}