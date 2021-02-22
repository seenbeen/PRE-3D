#pragma once
namespace GameObjectSubsystem
{
	class GameObject;

	class GameObjectComponent
	{
		friend class GameObject;

	public:
		GameObject& gameObject();

	protected:
		GameObjectComponent() = default;
		~GameObjectComponent() = default;

		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDestroy() = 0;

	private:
		GameObject* _pGameObject;
	};
}