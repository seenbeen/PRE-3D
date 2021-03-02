#pragma once
#include <include/modules/gameobject.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PREApplicationContext;

		class PREWorld;

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

		private:
			PREApplicationContext* _preApplicationContext = nullptr;

			using GameObjectTemplate::AddComponent;
		};
	} // namespace Core
} // namespace PRE