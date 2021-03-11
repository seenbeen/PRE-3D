#pragma once
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include <include/modules/animation.h>

namespace PRE
{
	namespace Core
	{
		class PREAnimatorComponent;
		
		class PRERendering;
		class PREAnimationConfig;

		using std::string;
		using std::unordered_map;
		using PRE::AnimationModule::Animation;

		class PREAnimation
		{
			PREAnimation& operator=(const PREAnimation&) = delete;
			PREAnimation(const PREAnimation&) = delete;

			friend class PRERendering;
			friend class PREAnimatorComponent;

		private:
			Animation _animation;

			PREAnimation(const PREAnimationConfig& animationConfig);
			~PREAnimation();

			static void GetBlendedStateAt(
				const PREAnimation& a,
				const PREAnimation& b,
				float timeA,
				float timeB,
				float blendFactor,
				unordered_map<string, glm::mat4>& result
			);

			float GetDuration() const;

			void GetStateAt(
				float time,
				unordered_map<string, glm::mat4>& result
			) const;
		};
	} // namespace Core
} // namespace PRE