#pragma once
#include <algorithm>
#include <list>

namespace PRE
{
	namespace Utility
	{
		namespace Math
		{
			using std::list;

			float TrueMod(float x, float y);

			template <typename TValue>
			TValue WeightedAverage(const list<TValue>& values)
			{
				TValue total = (TValue)0;
				auto n = (int)values.size();
				auto i = n;
				std::for_each(
					values.begin(),
					values.end(),
					[&total, &i](const auto& data)
					{
						total += data * i--;
					}
				);
				return total / (n * (n + 1.0f) / 2.0f);
			}
		}
	}
}