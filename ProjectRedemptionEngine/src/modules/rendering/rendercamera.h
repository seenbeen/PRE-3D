#pragma once
#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;

		class RenderCamera
		{
			RenderCamera& operator=(const RenderCamera&) = delete;
			RenderCamera(const RenderCamera&) = delete;

			friend class Renderer;

		public:
			enum class Kind { ORTHOGRAPHIC, PERSPECTIVE };

		private:
			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderCamera;

			private:
				static Impl& MakeImpl(
					const Kind& kind,
					float size,
					float aspectRatio,
					float nearClippingPlane,
					float farClippingPlane
				);

				bool projectionMatrixChanged;

				Kind kind;

				float size;
				float aspectRatio;
				float nearClippingPlane;
				float farClippingPlane;

				glm::mat4 viewMatrix;
				glm::mat4 projectionMatrix;

				Impl(
					const Kind& kind,
					float size,
					float aspectRatio,
					float nearClippingPlane,
					float farClippingPlane
				);
				~Impl();
			};

		public:
			void SetKind(const Kind& kind);
			const Kind& GetKind() const;

			void SetSize(float size);
			float GetSize() const;

			void SetAspectRatio(float aspectRatio);
			float GetAspectRatio() const;

			void SetNearClippingPlane(float nearClippingPlane);
			float GetNearClippingPlane() const;

			void SetFarClippingPlane(float farClippingPlane);
			float GetFarClippingPlane() const;

			void SetViewMatrix(const glm::mat4& viewMatrix);

			const glm::mat4& GetViewMatrix();
			const glm::mat4& GetProjectionMatrix();

		private:
			Impl& _impl;

			RenderCamera(
				const Kind& kind,
				float size,
				float aspectRatio,
				float nearClippingPlane,
				float farClippingPlane
			);
			~RenderCamera();
		};
	} // namespace RenderingModule
} // namespace PRE