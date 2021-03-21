#include <utility/math.h>

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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

			bool Decompose(const glm::mat4& matrix, glm::vec3 scale, glm::vec3 rotation, glm::fquat& rotationQuat, glm::vec3& position)
			{
				position = glm::vec3(matrix[3]);
				scale = glm::vec3(glm::length(matrix[0]), glm::length(matrix[1]), glm::length(matrix[2]));

				auto r11 = matrix[0].x / scale.x;
				auto r21 = matrix[0].y / scale.x;
				auto r31 = matrix[0].z / scale.x;
				auto r32 = matrix[1].z / scale.y;
				auto r33 = matrix[2].z / scale.z;

				rotation = glm::vec3(
					glm::atan(r32, r33),
					glm::atan(-r31, glm::sqrt(r32 * r32 + r33 * r33)),
					glm::atan(r21, r11)
				);
				rotationQuat = glm::fquat(rotation);
				rotation = glm::degrees(rotation);
				return true;
			}
		}
	}
}