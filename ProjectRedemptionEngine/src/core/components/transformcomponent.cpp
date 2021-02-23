#include <core/components/transformcomponent.h>

#include <iterator>
#include <list>
#include <queue>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <include/modules/transform.h>

using std::list;
using std::queue;

using PRE::TransformModule::Transform;

namespace PRE
{
	namespace Core
	{
		TransformComponent::TransformComponent() : _transform(*this) {}

#pragma region Relations
		TransformComponent* TransformComponent::GetParent()
		{
			return _transform.GetParent()->GetAssociation();
		}

		void TransformComponent::SetParent(TransformComponent* transform, bool worldPositionStays)
		{
			_transform.SetParent(
				transform != nullptr ? &transform->_transform : nullptr,
				worldPositionStays
			);
		}

		list<TransformComponent*> TransformComponent::GetChildren()
		{
			auto children = _transform.GetChildren();
			list<TransformComponent*> result;
			std::transform(
				children.begin(),
				children.end(),
				std::back_inserter(result),
				[](auto transform) { return transform->GetAssociation(); }
			);
			return result;
		}
#pragma endregion

#pragma region Position
		const glm::vec3& TransformComponent::GetPosition()
		{
			return _transform.GetPosition();
		}

		const glm::vec3& TransformComponent::GetLocalPosition()
		{
			return _transform.GetLocalPosition();
		}

		void TransformComponent::SetPosition(const glm::vec3& position)
		{
			_transform.SetPosition(position);
		}

		void TransformComponent::SetLocalPosition(const glm::vec3& localPosition)
		{
			_transform.SetLocalPosition(localPosition);
		}
#pragma endregion

#pragma region Rotation
		const glm::fquat& TransformComponent::GetRotation()
		{
			return _transform.GetRotation();
		}

		const glm::fquat& TransformComponent::GetLocalRotation()
		{
			return _transform.GetLocalRotation();
		}

		void TransformComponent::SetRotation(const glm::fquat& rotation)
		{
			_transform.SetRotation(rotation);
		}

		void TransformComponent::SetLocalRotation(const glm::fquat& localRotation)
		{
			_transform.SetLocalRotation(localRotation);
		}

		const glm::vec3& TransformComponent::GetEuler()
		{
			return _transform.GetEuler();
		}

		const glm::vec3& TransformComponent::GetLocalEuler()
		{
			return _transform.GetLocalEuler();
		}

		void TransformComponent::SetEuler(const glm::vec3& euler)
		{
			_transform.SetEuler(euler);
		}

		void TransformComponent::SetLocalEuler(const glm::vec3& localEuler)
		{
			_transform.SetLocalEuler(localEuler);
		}
#pragma endregion

#pragma region Scale
		const glm::vec3& TransformComponent::GetScale()
		{
			return _transform.GetScale();
		}

		const glm::vec3& TransformComponent::GetLocalScale()
		{
			return _transform.GetLocalScale();
		}

		void TransformComponent::SetScale(const glm::vec3& scale)
		{
			_transform.SetScale(scale);
		}

		void TransformComponent::SetLocalScale(const glm::vec3& localScale)
		{
			_transform.SetLocalScale(localScale);
		}
#pragma endregion

		const glm::mat4& TransformComponent::GetMatrix()
		{
			return _transform.GetMatrix();
		}

		const glm::mat4& TransformComponent::GetInverseMatrix()
		{
			return _transform.GetInverseMatrix();
		}

		void TransformComponent::OnDestroy()
		{
			SetParent(nullptr, true);
			queue<TransformComponent*> transformQueue;

			transformQueue.push(this);
			while (!transformQueue.empty())
			{
				auto top = transformQueue.front();
				transformQueue.pop();

				auto children = top->GetChildren();
				for (auto it = children.begin(); it != children.end(); ++it)
				{
					auto pChild = *it;
					pChild->SetParent(nullptr, true);
					transformQueue.push(pChild);
				}

				// this is fine; re-destruction of parent = no-op
				gameObject().Destroy(top->gameObject());
			}
		}
	} // namespace Core
} // namespace PRE