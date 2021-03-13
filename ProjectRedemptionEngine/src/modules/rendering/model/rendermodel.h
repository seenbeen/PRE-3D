#pragma once
#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderMesh;
		class RenderSkeleton;
		class RenderMaterial;

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
				RenderMesh* pMesh;
				RenderSkeleton* pSkeleton;
				RenderMaterial* pMaterial;

				static Impl& MakeImpl();

				Impl();
				~Impl();
			};

		public:
			glm::mat4 modelMatrix;

		private:
			Impl& _impl;

			RenderModel();
			~RenderModel();

			void SetMesh(RenderMesh* pMesh);
			void SetSkeleton(RenderSkeleton* pSkeleton);
			void SetMaterial(RenderMaterial* pMaterial);

			void Render(
				const glm::mat4& viewMatrix,
				const glm::mat4& projectionMatrix
			);
		};
	} // namespace RenderingModule
} // namespace PRE