#pragma once
#include <include/modules/time.h>

namespace PRE
{
	namespace Core
	{
		struct PRETimeConfig;

		class PREApplicationContext;

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
			static const float MILLIS_IN_SECOND;

			static PRETime& MakePRETime(
				const PRETimeConfig& timeConfig,
				PREApplicationContext& applicationContext
			);

			PREApplicationContext& _applicationContext;
			Time& _time;
			unsigned int _frameLimit;

			PRETime(PREApplicationContext& applicationContext, Time& time);
			~PRETime();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE