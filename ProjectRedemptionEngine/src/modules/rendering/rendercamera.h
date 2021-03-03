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
				bool projectionMatrixChanged;
				bool viewMatrixChanged;

				Kind kind;

				float size;
				float aspectRatio;
				float nearClippingPlane;
				float farClippingPlane;

				glm::mat4 viewMatrix;
				glm::mat4 projectionMatrix;
				glm::mat4 viewProjectionMatrix;

				static Impl& MakeImpl(
					const Kind& kind,
					float size,
					float aspectRatio,
					float nearClippingPlane,
					float farClippingPlane
				);

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
			const Kind& GetKind() const;
			void SetKind(const Kind& kind);

			float GetSize() const;
			void SetSize(float size);

			float GetAspectRatio() const;
			void SetAspectRatio(float aspectRatio);

			float GetNearClippingPlane() const;
			void SetNearClippingPlane(float nearClippingPlane);

			float GetFarClippingPlane() const;
			void SetFarClippingPlane(float farClippingPlane);

			void SetViewMatrix(const glm::mat4& viewMatrix);

			const glm::mat4& GetViewProjectionMatrix();

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