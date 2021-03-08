#pragma once
#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderMesh;
		class RenderBone;
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
				RenderBone* pRootBone;
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
			void SetRootBone(RenderBone* pRootBone);
			void SetMaterial(RenderMaterial* pMaterial);

			void Render(const glm::mat4& viewProjectionMatrix);
		};
	} // namespace RenderingModule
} // namespace PRE