#pragma once
#include <string>
#include <unordered_map>

#include <include/modules/rendering.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PRESkeleton;
		class PREAnimator;

		using std::string;
		using std::unordered_map;

		class PREAnimatorComponent : public PREGameObjectComponent
		{
		public:
			class Controller
			{
				Controller& operator=(const Controller&) = delete;
				Controller(const Controller&) = delete;

				friend class PREAnimatorComponent;

			public:
				void TransitionTo(string state, float blendDuration);
				void SetTime(float time);
				void SetSpeed(float speed);
				void SetClamp(bool clamp); // will stop the animation once it hits 0

				template<class TAnimatorContext>
				TAnimatorContext& GetAnimatorContextAs()
				{
					return *static_cast<TAnimatorContext*>(
						_animatorComponent._vAnimatorContext
					);
				}

			private:
				PREAnimatorComponent& _animatorComponent;

				Controller(PREAnimatorComponent& parentComponent);
				~Controller();
			};

			void SetAnimator(
				PREAnimator* pAnimator,
				const string& startState,
				void* vAnimatorContext,
				float startTime,
				float startSpeed,
				bool isClamped

			);

			void SetSkeleton(PRESkeleton* pSkeleton);

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			PRESkeleton* _pSkeleton = nullptr;

			Controller* _pController = nullptr;

			unordered_map<string, glm::mat4> _stateCache;

			PREAnimator* _pAnimator = nullptr;
			void* _vAnimatorContext = nullptr;

			string _previousState = "";
			float _previousStateTime = 0;

			string _currentState = "";
			float _currentStateTime = 0;

			float _blendTime = 0;
			float _blendDuration = 0;

			float _previousStateSpeed = 1.0f;
			float _currentStateSpeed = 1.0f;

			bool _isPreviousClamped = false;
			bool _isCurrentClamped = false;
		};
	} // namespace Core
} // namespace PRE