#pragma once
#include <list>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <include/modules/transform.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		using std::list;

		using PRE::TransformModule::Transform;

		class PRETransformComponent : public PREGameObjectComponent
		{
		public:
			PRETransformComponent();

#pragma region Relations
			PRETransformComponent* GetParent();
			void SetParent(PRETransformComponent* transform, bool worldPositionStays);

			list<PRETransformComponent*> GetChildren();
#pragma endregion

#pragma region Utilities
			glm::vec3 GetForward();
			glm::vec3 GetUp();
			glm::vec3 GetRight();
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

			const glm::mat4& GetMatrix() const;
			const glm::mat4& GetInverseMatrix() const;

		protected:
			void OnDestroy() override;

		private:
			Transform<PRETransformComponent> _transform;
		};
	} // namespace Core
} // namespace PRE