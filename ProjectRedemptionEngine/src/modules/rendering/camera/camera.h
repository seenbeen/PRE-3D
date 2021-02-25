#pragma once
#include <glm/glm.hpp>
#include <modules/transform/transform.h>

namespace PRE
{
	namespace RenderingModule
	{
		using PRE::TransformModule::Transform;

		class Camera
		{
		public:
			Camera();
			~Camera();

			Transform<Camera> transform;
			glm::mat4 GetTransform();

		protected:
			virtual const glm::mat4& GetProjectionMatrix() = 0;

		private:
			glm::mat4 _projectionMatrix;
		};
	} // namespace RenderingModule
} // namespace PRE