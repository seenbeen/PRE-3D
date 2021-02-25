#pragma once
#include <glm/glm.hpp>

#include <modules/rendering/camera/camera.h>

namespace PRE
{
	namespace RenderingModule
	{
		class PerspectiveCamera : public Camera
		{
		public:
			PerspectiveCamera(
				float size,
				float aspectRatio,
				float nearClippingPlane,
				float farClippingPlane
			);
			~PerspectiveCamera();

			float GetSize();
			void SetSize(float size);

			float GetAspectRatio();
			void SetAspectRatio(float aspectRatio);

			float GetNearClippingPlane();
			void SetNearClippingPlane(float nearClippingPlane);

			float GetFarClippingPlane();
			void SetFarClippingPlane(float farClippingPlane);

		protected:
			const glm::mat4& GetProjectionMatrix() override;

		private:
			float _size;
			float _aspectRatio;
			float _nearClippingPlane;
			float _farClippingPlane;

			glm::mat4 _projectionMatrix;

			void RefreshProjectionMatrix();
		};
	} // namespace RenderingModule
} // namespace PRE