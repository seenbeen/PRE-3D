#pragma once
#include <modules/rendering/camera/camera.h>

namespace PRE
{
	namespace RenderingModule
	{
		class OrthographicCamera : public Camera
		{
		public:
			OrthographicCamera(float size, float aspectRatio, float renderDistance);
			~OrthographicCamera() override;

			float GetSize();
			void SetSize(float size);

			float GetAspectRatio();
			void SetAspectRatio(float aspectRatio);

			float GetRenderDistance();
			void SetRenderDistance(float renderDistance);

		private:
			float _size;
			float _aspectRatio;
			float _renderDistance;

			void RefreshProjectionMatrix();
		};
	} // namespace RenderingModule
} // namespace PRE