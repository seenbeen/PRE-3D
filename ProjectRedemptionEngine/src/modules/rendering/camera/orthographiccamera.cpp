#include <modules/rendering/camera/orthographiccamera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		OrthographicCamera::OrthographicCamera(
			float size,
			float aspectRatio,
			float renderDistance
		) : 
			_size(size),
			_aspectRatio(aspectRatio),
			_renderDistance(renderDistance)
		{
			RefreshProjectionMatrix();
		}

		OrthographicCamera::~OrthographicCamera() {}

		float OrthographicCamera::GetSize()
		{
			return _size;
		}

		void OrthographicCamera::SetSize(float size)
		{
			_size = size;
			RefreshProjectionMatrix();
		}

		float OrthographicCamera::GetAspectRatio()
		{
			return _aspectRatio;
		}

		void OrthographicCamera::SetAspectRatio(float aspectRatio)
		{
			_aspectRatio = aspectRatio;
			RefreshProjectionMatrix();
		}

		float OrthographicCamera::GetRenderDistance()
		{
			return _renderDistance;
		}

		void OrthographicCamera::SetRenderDistance(float renderDistance)
		{
			_renderDistance = renderDistance;
			RefreshProjectionMatrix();
		}

		void OrthographicCamera::RefreshProjectionMatrix()
		{
			auto hAspect = _size * _aspectRatio;
			SetProjectionMatrix(
				glm::ortho(
					-_size,
					_size,
					-hAspect,
					hAspect,
					0.0f,
					_renderDistance
				)
			);
		}
	} // RenderingModule
} // PRE