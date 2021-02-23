#include <algorithm>
#include <list>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <core/components/transformcomponent.h>

namespace PRE
{
	namespace Core
	{
#pragma region Relations
		TransformComponent* TransformComponent::GetParent()
		{
			return _parentTransform;
		}

		void TransformComponent::SetParent(TransformComponent* transform, bool worldPositionStays)
		{
			if (transform == this)
			{
				throw "Cannot set Parent to self.";
			}
			else if (transform == _parentTransform)
			{
				return;
			}
			else
			{
				// detach from current parent
				if (_parentTransform != nullptr)
				{
					_parentTransform->_childTransforms.erase(
						std::find(
							_parentTransform->_childTransforms.begin(),
							_parentTransform->_childTransforms.end(),
							this
						)
					);
				}

				_parentTransform = transform;

				if (worldPositionStays)
				{
					UpdateMatrix();
				}
				else
				{
					UpdateLocalMatrix();
				}
			}
		}

		list<TransformComponent*> TransformComponent::GetChildren()
		{
			return _childTransforms;
		}
#pragma endregion

#pragma region Position
		const glm::vec3& TransformComponent::GetPosition()
		{

			return _position;
		}

		const glm::vec3& TransformComponent::GetLocalPosition()
		{

			return _localPosition;
		}

		void TransformComponent::SetPosition(const glm::vec3& position)
		{
			_position = position;
			UpdateMatrix();
		}

		void TransformComponent::SetLocalPosition(const glm::vec3& localPosition)
		{
			_localPosition = localPosition;
			UpdateLocalMatrix();
		}
#pragma endregion

#pragma region Rotation
		const glm::fquat& TransformComponent::GetRotation()
		{
			return _rotation;
		}

		const glm::fquat& TransformComponent::GetLocalRotation()
		{
			return _localRotation;
		}

		void TransformComponent::SetRotation(const glm::fquat& rotation)
		{
			_rotation = rotation;
			_euler = glm::eulerAngles(_rotation);
			UpdateMatrix();
		}

		void TransformComponent::SetLocalRotation(const glm::fquat& localRotation)
		{
			_localRotation = localRotation;
			_localEuler = glm::eulerAngles(_localRotation);
			UpdateLocalMatrix();
		}

		const glm::vec3& TransformComponent::GetEuler()
		{
			return _euler;
		}

		const glm::vec3& TransformComponent::GetLocalEuler()
		{
			return _localEuler;
		}

		void TransformComponent::SetEuler(const glm::vec3& euler)
		{
			_euler = euler;
			_rotation = glm::quat(_euler);
			UpdateMatrix();
		}

		void TransformComponent::SetLocalEuler(const glm::vec3& localEuler)
		{
			_localEuler = localEuler;
			_localRotation = glm::quat(_localEuler);
			UpdateLocalMatrix();
		}
#pragma endregion

#pragma region Scale
		const glm::vec3& TransformComponent::GetScale()
		{
			return _scale;
		}

		const glm::vec3& TransformComponent::GetLocalScale()
		{
			return _localScale;
		}

		void TransformComponent::SetScale(const glm::vec3& scale)
		{
			_scale = scale;
			UpdateMatrix();
		}

		void TransformComponent::SetLocalScale(const glm::vec3& localScale)
		{
			_localScale = localScale;
			UpdateLocalMatrix();
		}
#pragma endregion

		const glm::mat4& TransformComponent::GetMatrix()
		{
			return _matrix;
		}

		const glm::mat4& TransformComponent::GetInverseMatrix()
		{
			return _inverseMatrix;
		}

		void TransformComponent::UpdateMatrix()
		{
			_matrix = glm::scale(glm::mat4(), _scale);
			_matrix = glm::mat4_cast(_rotation) * _matrix;
			_matrix = glm::translate(_matrix, _position);

			if (_parentTransform != nullptr)
			{
				_localMatrix = _parentTransform->_inverseMatrix * _matrix;
			}
			else
			{
				_localMatrix = glm::mat4();
			}

			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(_localMatrix, _localScale, _localRotation, _localPosition, skew, perspective);
			_localEuler = glm::eulerAngles(_localRotation);

			// TODO: Cache
			_inverseMatrix = glm::inverse(_matrix);

			// Notify children of update
			for (auto it = _childTransforms.begin(); it != _childTransforms.end(); ++it)
			{
				(*it)->AncestryChanged();
			}
		}

		void TransformComponent::UpdateLocalMatrix()
		{
			_localMatrix = glm::scale(glm::mat4(), _localScale);
			_localMatrix = glm::mat4_cast(_localRotation) * _matrix;
			_localMatrix = glm::translate(_matrix, _localPosition);

			if (_parentTransform != nullptr)
			{
				_matrix = _parentTransform->_matrix * _localMatrix;
			}
			else
			{
				_matrix = _localMatrix;
			}

			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(_matrix, _scale, _rotation, _position, skew, perspective);
			_euler = glm::eulerAngles(_rotation);

			// TODO: Cache
			_inverseMatrix = glm::inverse(_matrix);

			// Notify children of update
			for (auto it = _childTransforms.begin(); it != _childTransforms.end(); ++it)
			{
				(*it)->AncestryChanged();
			}
		}

		void TransformComponent::AncestryChanged()
		{
			_matrix = _parentTransform->_matrix * _localMatrix;

			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(_matrix, _scale, _rotation, _position, skew, perspective);
			_euler = glm::eulerAngles(_rotation);

			for (auto it = _childTransforms.begin(); it != _childTransforms.end(); ++it)
			{
				(*it)->AncestryChanged();
			}
		}
	} // namespace Core
} // namespace PRE