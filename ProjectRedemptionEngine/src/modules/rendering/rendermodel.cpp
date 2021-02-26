#include <modules/rendering/rendermodel.h>

#include <modules/rendering/camera/camera.h>

namespace PRE
{
	namespace RenderingModule
	{
		void RenderModel::OnRender(const Camera& camera, CompositingTarget& compositingTarget)
		{
			auto mvp = camera.GetViewProjectionMatrix() * transform.GetMatrix();
			// do some fanciful rendering
		}
	} // namespace RenderingModule
} // namespace PRE