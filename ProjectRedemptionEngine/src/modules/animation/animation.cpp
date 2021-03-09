#include <modules/animation/animation.h>

#include <cmath>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include <modules/animation/animationconfig.h>
#include <modules/animation/animationchannelconfig.h>
#include <modules/animation/animationchannel.h>

namespace PRE
{
	namespace AnimationModule
	{
		using std::string;
		using std::unordered_map;

		Animation::Impl& Animation::Impl::MakeImpl(
			float ticksPerSecond,
			float duration,
			const AnimationConfig& animationConfig
		)
		{
			unordered_map<string, AnimationChannel*> channels;
			for (
				auto it = animationConfig._animationChannelConfigs.begin();
				it != animationConfig._animationChannelConfigs.end();
				++it
			)
			{
				channels[(*it)->channelName] = new AnimationChannel(**it);
			}
			return *(new Impl(ticksPerSecond, duration, channels));
		}

		Animation::Impl::Impl(
			float ticksPerSecond,
			float duration,
			unordered_map<string, AnimationChannel*>& channels
		)
			:
			ticksPerSecond(ticksPerSecond),
			duration(duration),
			channels(std::move(channels)) {}

		Animation::Impl::~Impl()
		{
			for (auto it = channels.begin(); it != channels.end(); ++it)
			{
				delete it->second;
			}
		}

		Animation::Animation(
			float ticksPerSecond,
			float duration,
			AnimationConfig& animationConfig
		)
			:
			_impl(Impl::MakeImpl(ticksPerSecond, duration, animationConfig)) {}

		Animation::~Animation()
		{
			delete& _impl;
		}

		void Animation::GetBlendedStateAt(
			const Animation& a,
			const Animation& b,
			float timeA,
			float timeB,
			float blendFactor,
			unordered_map<string, glm::mat4>& result
		)
		{
			result.clear();

			timeA = std::fmod(timeA * a._impl.ticksPerSecond, a._impl.duration);
			timeB = std::fmod(timeB * a._impl.ticksPerSecond, b._impl.duration);
			blendFactor = std::fmod(blendFactor, 1.0f);

			// arbitrarily use a as baseline
			for (
				auto itA = a._impl.channels.begin();
				itA != a._impl.channels.end();
				++itA
			)
			{
				auto itB = b._impl.channels.find(itA->first);
				if (itB != b._impl.channels.end())
				{
					result[itA->first] = AnimationChannel::GetBlendedStateAt(
						*itA->second,
						*itB->second,
						timeA,
						timeB,
						blendFactor
					);
				}
				else
				{
					// take 100% of A's state if channel doesn't exist in B
					result[itA->first] = itA->second->GetStateAt(timeA);
				}
			}

			// fill in all leftover channels from b
			for (
				auto itB = b._impl.channels.begin();
				itB != b._impl.channels.end();
				++itB
			)
			{
				auto itA = a._impl.channels.find(itB->first);
				// this time we're only interested in channels only in B
				if (itA == a._impl.channels.end())
				{
					result[itB->first] = itB->second->GetStateAt(timeB);
				}
			}
		}

		float Animation::GetDuration() const
		{
			return _impl.duration / _impl.ticksPerSecond;
		}

		void Animation::GetStateAt(
			float time,
			unordered_map<string, glm::mat4>& result
		) const
		{
			result.clear();
			time = std::fmod(time * _impl.ticksPerSecond, _impl.duration);
			for (auto it = _impl.channels.begin(); it != _impl.channels.end(); ++it)
			{
				result[it->first] = it->second->GetStateAt(time);
			}
		}
	} // namespace AnimationModule
} // namespace PRE