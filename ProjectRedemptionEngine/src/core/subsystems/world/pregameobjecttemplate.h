#pragma once
#include <include/modules/gameobject.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PREApplicationContext;

		class PRERendering;

		using PRE::GameObjectModule::GameObjectTemplate;

		class PREGameObjectTemplate : public GameObjectTemplate
		{
			friend class PREWorld;

		protected:
			template<class TPREGameObjectComponent>
			TPREGameObjectComponent* AddPREComponent()
			{
				auto newComponent = GameObjectTemplate::AddComponent<TPREGameObjectComponent>();
				static_cast<PREGameObjectComponent*>(newComponent)->_preApplicationContext = _preApplicationContext;
				return newComponent;
			}

			template<class TPREGameObjectComponent>
			TPREGameObjectComponent* GetPREComponent()
			{
				auto pComponent = GetComponent<TPREGameObjectComponent>();
#ifdef __PRE_DEBUG__
				static_cast<PREGameObjectComponent*>(pComponent);
#endif
				return pComponent;
			}

			void OnInstantiate() override;

			virtual void OnInstantiateTemplate() = 0;

		private:
			PREApplicationContext* _preApplicationContext = nullptr;

			using GameObjectTemplate::AddComponent;
			using GameObjectTemplate::GetComponent;
		};
	} // namespace Core
} // namespace PRE