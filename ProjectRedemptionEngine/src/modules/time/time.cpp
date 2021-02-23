#include <modules/time/time.h>

#include <chrono>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

namespace PRE
{
	namespace TimeModule
	{
		Time::Time()
		{
			_deltaTime = 0.0f;
			_lastFrameMillis = 0.0;
		}

		Time::~Time() {}

		void Time::Update()
		{
			if (_lastFrameMillis == 0.0)
			{
				_lastFrameMillis = duration_cast<milliseconds>(
					system_clock::now().time_since_epoch()
				)
				.count();
			}
	
			auto currentFrameMillis = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
			)
			.count();
			_deltaTime = (currentFrameMillis - _lastFrameMillis) / Time::MILLIS_IN_SECOND;
			currentFrameMillis = _lastFrameMillis;
		}

		float Time::GetDeltaTime()
		{
			return _deltaTime;
		}
	}
}