#pragma once
#include <set>
#include <unordered_set>

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

		using std::set;
		using std::unordered_set;

		using PRE::RenderingModule::RenderModel;

		class PREModelRendererComponent : public PREGameObjectComponent
		{
			friend class PRERendering;

		public:
			void SetMaterial(PREMaterial* pMaterial);
			PREMaterial* GetMaterial() const;

			void SetMesh(PREMesh* pMesh);
			PREMesh* GetMesh() const;

			void SetSkeleton(PRESkeleton* pSkeleton);
			PRESkeleton* GetSkeleton() const;

			void AddCameraComponent(PRECameraComponent& cameraComponent);
			void RemoveCameraComponent(PRECameraComponent& cameraComponent);

			void AddAffectedLightLayer(int lightLayer);
			void RemoveAffectedLightLayer(int lightLayer);

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			bool _hasChanged = false;

			PRETransformComponent* _pTransformComponent = nullptr;
			RenderModel* _pModel;

			unordered_set<PRECameraComponent*> _pCameraComponents;
			set<int> _affectedLightLayers;

			PREMaterial* _pMaterial = nullptr;
			PREMesh* _pMesh = nullptr;
			PRESkeleton* _pSkeleton = nullptr;

			void AllocateIfNotAllocated();
			void DeallocateIfAllocated();
		};
	} // namespace Core
} // namespace PRE