#pragma once
#include <glm/glm.hpp>

#include <modules/transform/transform.h>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class Camera;
		class CompositingTarget;

		using PRE::TransformModule::Transform;

		class RenderModel
		{
			friend class Renderer;

		public:
			Transform<RenderModel> transform;

		private:
			// RenderMaterial material;
			// RenderMesh mesh;
			// list<RenderShader*> shaders;
			// list<RenderBone*> bones;

			void OnRender(const Camera& camera, CompositingTarget& compositingTexture);
		};
	} // namespace RenderingModule
} // namespace PRE