#pragma once
#include <glm/glm.hpp>

#include <modules/rendering/camera/camera.h>

namespace PRE
{
	namespace RenderingModule
	{
		class OrthographicCamera : public Camera
		{
		public:
			OrthographicCamera(float size, float aspectRatio, float renderDistance);
			~OrthographicCamera();

			float GetSize();
			void SetSize(float size);

			float GetAspectRatio();
			void SetAspectRatio(float aspectRatio);

			float GetRenderDistance();
			void SetRenderDistance(float renderDistance);

		protected:
			const glm::mat4& GetProjectionMatrix() override;

		private:
			float _size;
			float _aspectRatio;
			float _renderDistance;

			glm::mat4 _projectionMatrix;

			void RefreshProjectionMatrix();
		};
	} // namespace RenderingModule
} // namespace PRE