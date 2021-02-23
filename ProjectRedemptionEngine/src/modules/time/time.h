#pragma once
#include <chrono>

namespace PRE
{
	namespace TimeModule
	{
		class Time
		{
		public:
			Time();
			~Time();

			void Update();

			float GetDeltaTime();

		private:
			const long double MILLIS_IN_SECOND = 1000.0l;
			float _deltaTime;
			long double _lastFrameMillis;
		};
	}
}