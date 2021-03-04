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
		class PRETransformComponent;

		class PRERendering;
		class PREMaterial;
		class PREMesh;

		using PRE::RenderingModule::RenderModel;

		class PREModelRendererComponent : public PREGameObjectComponent
		{
			friend class PRERendering;

		public:
			void SetTagGroup(unsigned int tagGroup);
			unsigned int GetTagGroup() const;

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

			unsigned int _oldTagGroup = 0;
			unsigned int _tagGroup = 0;
			PREMaterial* _pMaterial = nullptr;
			PREMesh* _pMesh = nullptr;
		};
	} // namespace Core
} // namespace PRE