#pragma once
#include <chrono>

namespace PRE
{
	namespace TimeModule
	{
		class Time
		{
			Time& operator=(const Time&) = delete;
			Time(const Time&) = delete;

		public:
			Time();
			~Time();

			void Update();

			float GetDeltaTime();

		private:
			const long double MILLIS_IN_SECOND = 1000.0l;
			float _deltaTime;
			long long _lastFrameMillis;
		};
	}
}