#include <core/components/preanimatorcomponent.h>

#include <string>

#include <include/modules/rendering.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

#include <core/subsystems/rendering/preanimator.h>

namespace PRE
{
	namespace Core
	{
		void PREAnimatorComponent::Controller::TransitionTo(
			string state,
			float blendDuration
		)
		{
		
		}

		void PREAnimatorComponent::Controller::SetTime(float time)
		{

		}

		void PREAnimatorComponent::Controller::SetSpeed(float speed)
		{

		}

		void PREAnimatorComponent::Controller::SetClamp(bool clamp)
		{

		}

		PREAnimatorComponent::Controller::Controller(
			PREAnimatorComponent& parentComponent
		)
			:
			parentComponent(parentComponent) {}

		PREAnimatorComponent::Controller::~Controller() {}

		void PREAnimatorComponent::SetAnimator(
			PREAnimator* pAnimator,
			void* vAnimatorContext)
		{
		}

		void PREAnimatorComponent::OnStart()
		{

		}

		void PREAnimatorComponent::OnUpdate()
		{

		}

		void PREAnimatorComponent::OnDestroy()
		{

		}
	} // namespace Core
} // namespace PRE