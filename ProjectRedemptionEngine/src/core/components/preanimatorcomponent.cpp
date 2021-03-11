#include <core/components/preanimatorcomponent.h>

#include <algorithm>
#include <string>

#include <include/utility.h>

#include <include/modules/rendering.h>

#include <core/subsystems/time/pretime.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

#include <core/subsystems/rendering/preanimator.h>
#include <core/subsystems/rendering/preanimation.h>
#include <core/subsystems/rendering/preskeleton.h>

namespace PRE
{
	namespace Core
	{
		using namespace PRE::Utility;

		void PREAnimatorComponent::Controller::TransitionTo(
			string state,
			float blendDuration
		)
		{
			_animatorComponent._previousState = _animatorComponent._currentState;
			_animatorComponent._previousStateTime = _animatorComponent._currentStateTime;
			_animatorComponent._previousStateSpeed = _animatorComponent._currentStateSpeed;
			_animatorComponent._isPreviousClamped = _animatorComponent._isCurrentClamped;
			_animatorComponent._currentState = state;
			_animatorComponent._blendDuration = blendDuration;
			_animatorComponent._blendTime = 0;
		}

		void PREAnimatorComponent::Controller::SetTime(float time)
		{
			_animatorComponent._currentStateTime = time;
		}

		void PREAnimatorComponent::Controller::SetSpeed(float speed)
		{
			_animatorComponent._currentStateSpeed = speed;
		}

		void PREAnimatorComponent::Controller::SetClamp(bool clamp)
		{
			_animatorComponent._isCurrentClamped = clamp;
		}

		PREAnimatorComponent::Controller::Controller(
			PREAnimatorComponent& animatorComponent
		)
			:
			_animatorComponent(animatorComponent) {}

		PREAnimatorComponent::Controller::~Controller() {}

		void PREAnimatorComponent::SetAnimator(
			PREAnimator* pAnimator,
			const string& startState,
			void* vAnimatorContext,
			float startTime,
			float startSpeed,
			bool isClamped
		)
		{
			_pAnimator = pAnimator;
			_currentState = startState;
			_vAnimatorContext = vAnimatorContext;
			_currentStateTime = startTime;
			_currentStateSpeed = startSpeed;
			_isCurrentClamped = isClamped;
		}

		void PREAnimatorComponent::SetSkeleton(PRESkeleton* pSkeleton)
		{
			_pSkeleton = pSkeleton;
		}

		void PREAnimatorComponent::OnStart()
		{
			_pController = new Controller(*this);
		}

		void PREAnimatorComponent::OnUpdate()
		{
			if (_pAnimator != nullptr)
			{
				auto pState = _pAnimator->_states.find(_currentState)->second;
				pState.first(*_pController);

				auto deltaTime = GetTime().GetDeltaTime();
				auto currentStateDuration = pState.second->GetDuration();
				if (_isCurrentClamped)
				{
					_currentStateTime = std::max(
						0.0f,
						std::min(
							_currentStateTime + deltaTime * _currentStateSpeed,
							currentStateDuration
						)
					);
				}
				else
				{
					_currentStateTime = Math::TrueMod(
						_currentStateTime + deltaTime * _currentStateSpeed,
						currentStateDuration
					);
				}

				_blendTime = std::min(_blendTime + deltaTime, _blendDuration);
				if (_blendTime < _blendDuration)
				{
					auto pPrevState = _pAnimator->_states.find(_previousState)->second;
					auto prevStateDuration = pPrevState.second->GetDuration();
					if (_isPreviousClamped)
					{
						_previousStateTime = std::max(
							0.0f,
							std::min(
								_previousStateTime + deltaTime * _previousStateSpeed,
								prevStateDuration
							)
						);
					}
					else
					{
						_previousStateTime = Math::TrueMod(
							_previousStateTime + deltaTime * _previousStateSpeed,
							prevStateDuration
						);
					}
				}

				if (_pSkeleton != nullptr)
				{
					if (_blendTime < _blendDuration)
					{
						auto pPrevState = _pAnimator->_states.find(_previousState)->second;
						PREAnimation::GetBlendedStateAt(
							*pPrevState.second,
							*pState.second,
							_previousStateTime,
							_currentStateTime,
							_blendTime / _blendDuration,
							_stateCache
						);
					}
					else
					{
						pState.second->GetStateAt(_currentStateTime, _stateCache);
					}

					for (auto it = _stateCache.begin(); it != _stateCache.end(); ++it)
					{
						_pSkeleton->SetBoneLocalMatrix(it->first, it->second);
					}
				}
			}
		}

		void PREAnimatorComponent::OnDestroy()
		{
			delete _pController;
		}
	} // namespace Core
} // namespace PRE