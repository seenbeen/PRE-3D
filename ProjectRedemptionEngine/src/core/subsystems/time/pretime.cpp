#include <core/subsystems/time/pretime.h>

#include <include/modules/time.h>

namespace PRE
{
	namespace Core
	{
		float PRETime::GetDeltaTime()
		{
			return _time->GetDeltaTime();
		}

		PRETime::PRETime()
			: _time(new Time()) {}

		PRETime::~PRETime()
		{
			delete _time;
		}

		void PRETime::Initialize() {}

		void PRETime::Update()
		{
			_time->Update();
		}

		void PRETime::Shutdown() {}
	}
}