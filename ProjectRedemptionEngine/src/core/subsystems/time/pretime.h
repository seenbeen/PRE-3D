#pragma once
#include <list>

#include <include/modules/time.h>

namespace PRE
{
	namespace Core
	{
		struct PRETimeConfig;

		class PREApplicationContext;

		using std::list;

		using PRE::TimeModule::Time;

		class PRETime
		{
			PRETime& operator=(const PRETime&) = delete;
			PRETime(const PRETime&) = delete;

			friend class PREApplicationContext;

		public:
			void SetFrameLimit(unsigned int frameLimit);
			unsigned int GetFrameLimit();

			float GetDeltaTime();

		private:
			static const long long NANOS_IN_SECOND;
			static const int FRAME_DELAY_SAMPLES;

			static PRETime& MakePRETime(
				const PRETimeConfig& timeConfig,
				PREApplicationContext& applicationContext
			);

			PREApplicationContext& _applicationContext;
			Time& _time;
			float _deltaTime;
			unsigned int _frameLimit;
			long double _secondsPerFrame;

			long double _priorFramesTimesSum;
			list<long double> _priorFramesTimes;

			PRETime(PREApplicationContext& applicationContext, Time& time);
			~PRETime();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE