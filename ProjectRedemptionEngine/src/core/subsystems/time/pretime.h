#pragma once

namespace PRE
{
	namespace TimeModule
	{
		class Time;
	} // namespace TimeModule
} // namespace PRE

namespace PRE
{
	namespace Core
	{
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
			Time* const _time;

			PRETime();
			~PRETime();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE