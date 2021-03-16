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

			long double GetDeltaTime();

		private:
			const long double NANOS_IN_SECOND = 1000000000.0l;
			long double _deltaTime;
			long long _lastFrameNanos;
		};
	}
}