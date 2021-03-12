#pragma once
#include <include/modules/rendering.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PRECameraComponent;
		class PRETransformComponent;

		class PREMaterial;
		class PREMesh;
		class PRESkeleton;

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

			void SetSkeleton(PRESkeleton* pSkeleton);
			PRESkeleton* GetSkeleton() const;

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			bool _hasChanged = false;

			PRETransformComponent* _pTransformComponent = nullptr;
			RenderModel* _pModel = nullptr;

			PRECameraComponent* _pCurrentCameraComponent = nullptr;
			PRECameraComponent* _pNextCameraComponent = nullptr;

			PREMaterial* _pMaterial = nullptr;
			PREMesh* _pMesh = nullptr;
			PRESkeleton* _pSkeleton = nullptr;
		};
	} // namespace Core
} // namespace PRE