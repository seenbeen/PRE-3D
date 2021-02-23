#pragma once
namespace PRE
{
	namespace GameObjectSubsystem
	{
		class GameObject;

		class GameObjectComponent
		{
			GameObjectComponent& operator=(const GameObjectComponent&) = delete;
			GameObjectComponent(const GameObjectComponent&) = delete;

			friend class GameObject;

		public:
			GameObject& gameObject();

		protected:
			GameObjectComponent() = default;
			~GameObjectComponent() = default;

			virtual void OnStart();
			virtual void OnUpdate();
			virtual void OnDestroy();

		private:
			GameObject* _pGameObject;
		};
	} // namespace GameObjectSubsystem
} // namespace PRE