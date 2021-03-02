#include <core/components/pretransformcomponent.h>

#include <iterator>
#include <list>
#include <queue>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <include/modules/transform.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		using std::list;
		using std::queue;

		using PRE::TransformModule::Transform;

		PRETransformComponent::PRETransformComponent() : _transform(*this) {}

#pragma region Relations
		PRETransformComponent* PRETransformComponent::GetParent()
		{
			return &_transform.GetParent()->GetAssociation();
		}

		void PRETransformComponent::SetParent(PRETransformComponent* transform, bool worldPositionStays)
		{
			_transform.SetParent(
				transform != nullptr ? &transform->_transform : nullptr,
				worldPositionStays
			);
		}

		list<PRETransformComponent*> PRETransformComponent::GetChildren()
		{
			auto children = _transform.GetChildren();
			list<PRETransformComponent*> result;
			std::transform(
				children.begin(),
				children.end(),
				std::back_inserter(result),
				[](auto transform) { return &transform->GetAssociation(); }
			);
			return result;
		}
#pragma endregion

#pragma region Position
		const glm::vec3& PRETransformComponent::GetPosition()
		{
			return _transform.GetPosition();
		}

		const glm::vec3& PRETransformComponent::GetLocalPosition()
		{
			return _transform.GetLocalPosition();
		}

		void PRETransformComponent::SetPosition(const glm::vec3& position)
		{
			_transform.SetPosition(position);
		}

		void PRETransformComponent::SetLocalPosition(const glm::vec3& localPosition)
		{
			_transform.SetLocalPosition(localPosition);
		}
#pragma endregion

#pragma region Rotation
		const glm::fquat& PRETransformComponent::GetRotation()
		{
			return _transform.GetRotation();
		}

		const glm::fquat& PRETransformComponent::GetLocalRotation()
		{
			return _transform.GetLocalRotation();
		}

		void PRETransformComponent::SetRotation(const glm::fquat& rotation)
		{
			_transform.SetRotation(rotation);
		}

		void PRETransformComponent::SetLocalRotation(const glm::fquat& localRotation)
		{
			_transform.SetLocalRotation(localRotation);
		}

		const glm::vec3& PRETransformComponent::GetEuler()
		{
			return _transform.GetEuler();
		}

		const glm::vec3& PRETransformComponent::GetLocalEuler()
		{
			return _transform.GetLocalEuler();
		}

		void PRETransformComponent::SetEuler(const glm::vec3& euler)
		{
			_transform.SetEuler(euler);
		}

		void PRETransformComponent::SetLocalEuler(const glm::vec3& localEuler)
		{
			_transform.SetLocalEuler(localEuler);
		}
#pragma endregion

#pragma region Scale
		const glm::vec3& PRETransformComponent::GetScale()
		{
			return _transform.GetScale();
		}

		const glm::vec3& PRETransformComponent::GetLocalScale()
		{
			return _transform.GetLocalScale();
		}

		void PRETransformComponent::SetScale(const glm::vec3& scale)
		{
			_transform.SetScale(scale);
		}

		void PRETransformComponent::SetLocalScale(const glm::vec3& localScale)
		{
			_transform.SetLocalScale(localScale);
		}
#pragma endregion

		const glm::mat4& PRETransformComponent::GetMatrix() const
		{
			return _transform.GetMatrix();
		}

		const glm::mat4& PRETransformComponent::GetInverseMatrix() const
		{
			return _transform.GetInverseMatrix();
		}

		void PRETransformComponent::OnDestroy()
		{
			SetParent(nullptr, true);
			queue<PRETransformComponent*> transformQueue;

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