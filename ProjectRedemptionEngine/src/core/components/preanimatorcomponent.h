#pragma once
#include <string>

#include <include/modules/rendering.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PREAnimator;

		using std::string;

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
				TAnimatorContext* GetPAnimatorContextAs()
				{
					return static_cast<TAnimatorContext*>(
						parentComponent.pAnimatorContext
					);
				}

			private:
				PREAnimatorComponent& parentComponent;

				Controller(PREAnimatorComponent& parentComponent);
				~Controller();
			};

			void SetAnimator(PREAnimator* pAnimator, void* vAnimatorContext);

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			PREAnimator* pAnimator;
			void* pAnimatorContext;

			string previousState;
			float previousStateTime;

			string currentState;
			float currentStateTime;

			float blendTime;
			float blendDuration;

			float previousStateSpeed;
			float currentStateSpeed;

			bool isPreviousClamped;
			bool isCurrentClamped;
		};
	} // namespace Core
} // namespace PRE