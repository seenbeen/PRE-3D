#include <modules/time/time.h>

#include <chrono>

namespace PRE
{
	namespace TimeModule
	{
		using std::chrono::duration_cast;
		using std::chrono::nanoseconds;
		using std::chrono::system_clock;

		Time::Time()
		{
			_deltaTime = 0.0l;
			_lastFrameNanos = 0l;
		}

		Time::~Time() {}

		void Time::Update()
		{
			if (_lastFrameNanos == 0l)
			{
				_lastFrameNanos = duration_cast<nanoseconds>(
					system_clock::now().time_since_epoch()
				)
				.count();
			}
	
			auto currentFrameNanos = duration_cast<nanoseconds>(
				system_clock::now().time_since_epoch()
			)
			.count();
			_deltaTime = (currentFrameNanos - _lastFrameNanos) / Time::NANOS_IN_SECOND;
			_lastFrameNanos = currentFrameNanos;
		}

		long double Time::GetDeltaTime()
		{
			return _deltaTime;
		}
	}
}