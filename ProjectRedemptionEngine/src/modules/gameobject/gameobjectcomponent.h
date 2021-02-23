#pragma once
namespace PRE
{
	namespace GameObjectModule
	{
		class GameObject;

		class GameObjectComponent
		{
			GameObjectComponent& operator=(const GameObjectComponent&) = delete;
			GameObjectComponent(const GameObjectComponent&) = delete;

			friend class GameObject;

		public:
			GameObjectComponent() = default;
			virtual ~GameObjectComponent() = default;

			GameObject& gameObject();

		protected:
			virtual void OnStart();
			virtual void OnUpdate();
			virtual void OnDestroy();

		private:
			GameObject* _pGameObject = nullptr;
		};
	} // namespace GameObjectSubsystem
} // namespace PRE