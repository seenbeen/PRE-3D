#pragma once
#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class CompositingTarget;

		class RenderMesh;
		class RenderMaterial;

		class Camera;
		class Renderer;

		class RenderModel
		{
			RenderModel& operator=(const RenderModel&) = delete;
			RenderModel(const RenderModel&) = delete;

			friend class Renderer;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderModel;

			private:
				RenderMaterial* material;
				RenderMesh* mesh;

				static Impl& MakeImpl(RenderMesh& mesh, RenderMaterial& material);

				Impl(RenderMesh& mesh, RenderMaterial& material);
				~Impl();
			};

		public:
			glm::mat4 modelMatrix;

		private:
			Impl& _impl;

			const unsigned int _renderTag;

			RenderModel(const unsigned int renderTag, RenderMesh& mesh, RenderMaterial& material);
			~RenderModel();

			void SetMesh(RenderMesh& mesh);
			void SetMaterial(RenderMaterial& material);

			void Render(const Camera& camera);
		};
	} // namespace RenderingModule
} // namespace PRE