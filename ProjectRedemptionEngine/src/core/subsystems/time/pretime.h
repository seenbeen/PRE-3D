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
			float GetDeltaTime();

		private:
			static PRETime& MakePRETime(
				const PRETimeConfig& timeConfig,
				PREApplicationContext& applicationContext
			);

			PREApplicationContext& _applicationContext;
			Time& _time;

			PRETime(PREApplicationContext& applicationContext, Time& time);
			~PRETime();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE