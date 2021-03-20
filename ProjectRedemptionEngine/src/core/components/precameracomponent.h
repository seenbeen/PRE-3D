#pragma once
#include <unordered_set>

#include <include/modules/rendering.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PRETransformComponent;
		class PREModelRendererComponent;

		class PRERenderTexture;
		class PRESkyBox;

		using std::unordered_set;

		using PRE::RenderingModule::RenderCamera;
		using PRE::RenderingModule::RenderCompositingNode;

		class PRECameraComponent : public PREGameObjectComponent
		{
			friend class PRERendering;

		public:
			enum class Kind { ORTHOGRAPHIC, PERSPECTIVE };

			void SetKind(Kind kind);
			Kind Getkind() const;

			void SetSize(float size);
			float GetSize() const;

			void SetAspectRatio(float aspectRatio);
			float GetAspectRatio() const;

			void SetNearClippingPlane(float nearClippingPlane);
			float GetNearClippingPlane() const;

			void SetFarClippingPlane(float farClippingPlane);
			float GetFarClippingPlane() const;

			void SetRenderTexture(PRERenderTexture* pRenderTexture);
			PRERenderTexture* GetRenderTexture() const;

			void SetSkyBox(PRESkyBox* pSkyBox);
			PRESkyBox* GetSkyBox();

			void SetTag(int tag);
			int GetTag();

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			bool _hasChanged = false;

			int _tag = 0;

			PRETransformComponent* _pTransformComponent = nullptr;
			RenderCamera* _pCamera = nullptr;

			Kind _kind = Kind::PERSPECTIVE;

			float _size = 5.0f;
			float _aspectRatio = 800.0f / 600.0f;
			float _nearClippingPlane = 0.1f;
			float _farClippingPlane = 100.0f;

			PRERenderTexture* _pRenderTexture = nullptr;
			PRESkyBox* _pSkyBox = nullptr;

			void AllocateIfNotAllocated();
			void DeallocateIfAllocated();
		};
	} // namespace Core
} // namespace PRE