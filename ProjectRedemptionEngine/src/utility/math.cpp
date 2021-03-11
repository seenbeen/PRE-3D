#include <utility/math.h>

#include <cmath>

namespace PRE
{
	namespace Utility
	{
		namespace Math
		{
			float TrueMod(float x, float y)
			{
				return std::fmod(std::fmod(x, y) + y, y);
			}
		}
	}
}