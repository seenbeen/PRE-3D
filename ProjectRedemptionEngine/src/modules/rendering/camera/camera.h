#pragma once
#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class Camera
		{
		public:
			Camera();
			virtual ~Camera() = 0;

			void SetViewMatrix(const glm::mat4& viewMatrix);

			const glm::mat4& GetViewProjectionMatrix() const;

		protected:
			void SetProjectionMatrix(const glm::mat4& projectionMatrix);

		private:
			glm::mat4 _viewMatrix;
			glm::mat4 _projectionMatrix;
			glm::mat4 _viewProjectionMatrix;
		};
	} // namespace RenderingModule
} // namespace PRE