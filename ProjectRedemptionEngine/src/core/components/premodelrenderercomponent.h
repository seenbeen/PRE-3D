#pragma once
#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace RenderingModule
	{
		class RenderModel;
	} // namespace RenderingModule
} // namespace PRE

namespace PRE
{
	namespace Core
	{
		class PRECameraComponent;
		class PRETransformComponent;

		class PRERendering;
		class PREMaterial;
		class PREMesh;

		using PRE::RenderingModule::RenderModel;

		class PREModelRendererComponent : public PREGameObjectComponent
		{
			friend class PRERendering;

		public:
			void SetCameraComponent(PRECameraComponent* pCameraComponent);
			PRECameraComponent* GetCameraComponent() const;

			void SetMaterial(PREMaterial* pMaterial);
			PREMaterial* GetMaterial() const;

			void SetMesh(PREMesh* pMesh);
			PREMesh* GetMesh() const;

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			bool _hasChanged = false;

			PRETransformComponent* _pTransformComponent = nullptr;
			RenderModel* _pModel = nullptr;

			PRECameraComponent* _pPreviousCameraComponent = nullptr;
			PRECameraComponent* _pCameraComponent = nullptr;

			PREMaterial* _pMaterial = nullptr;
			PREMesh* _pMesh = nullptr;
		};
	} // namespace Core
} // namespace PRE