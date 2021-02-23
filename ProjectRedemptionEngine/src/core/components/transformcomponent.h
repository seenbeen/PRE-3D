#pragma once
#include <list>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <include/subsystems/gameobject.h>
#include <subsystems/transform/transform.h>

using std::list;

using PRE::GameObjectSubsystem::GameObjectComponent;
using PRE::TransformSubsystem::Transform;

namespace PRE
{
	namespace Core
	{
		class TransformComponent : public GameObjectComponent
		{
		public:
			TransformComponent();

#pragma region Relations
			TransformComponent* GetParent();
			void SetParent(TransformComponent* transform, bool worldPositionStays);

			list<TransformComponent*> GetChildren();
#pragma endregion

#pragma region Position
			const glm::vec3& GetPosition();
			const glm::vec3& GetLocalPosition();

			void SetPosition(const glm::vec3& position);
			void SetLocalPosition(const glm::vec3& localPosition);
#pragma endregion

#pragma region Rotation
			const glm::fquat& GetRotation();
			const glm::fquat& GetLocalRotation();

			void SetRotation(const glm::fquat& rotation);
			void SetLocalRotation(const glm::fquat& localRotation);

			const glm::vec3& GetEuler();
			const glm::vec3& GetLocalEuler();

			void SetEuler(const glm::vec3& euler);
			void SetLocalEuler(const glm::vec3& localEuler);
#pragma endregion

#pragma region Scale
			const glm::vec3& GetScale();
			const glm::vec3& GetLocalScale();

			void SetScale(const glm::vec3& scale);
			void SetLocalScale(const glm::vec3& localScale);
#pragma endregion

			const glm::mat4& GetMatrix();
			const glm::mat4& GetInverseMatrix();

		protected:

		private:
			Transform<TransformComponent> _transform;
		};
	} // namespace Core
} // namespace PRE