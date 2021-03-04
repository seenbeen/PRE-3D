#pragma once
#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderMesh;
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
				RenderMaterial* pMaterial;
				RenderMesh* pMesh;

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
			void SetMaterial(RenderMaterial* pMaterial);

			void Render(const glm::mat4& viewProjectionMatrix);
		};
	} // namespace RenderingModule
} // namespace PRE