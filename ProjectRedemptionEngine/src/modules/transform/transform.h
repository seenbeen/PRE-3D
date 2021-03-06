#pragma once
#include <algorithm>
#include <list>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace PRE
{
	namespace TransformModule
	{
		using std::list;

		template<class TNodeAssociation>
		class Transform
		{
			Transform& operator=(const Transform&) = delete;
			Transform(const Transform&) = delete;

		public:
			Transform(TNodeAssociation& association)
				:
				_association(association),
				_parentTransform(nullptr),
				_matrix(),
				_inverseMatrix(),
				_localMatrix(),
				_position(),
				_localPosition(),
				_rotation(glm::vec3()),
				_localRotation(glm::vec3()),
				_euler(),
				_localEuler(),
				_scale(1),
				_localScale(1) {}

			~Transform()
			{
				for (auto it = _childTransforms.begin(); it != _childTransforms.end(); ++it)
				{
					(*it)->SetParent(nullptr, true);
				}
				SetParent(nullptr, true);
			}

			TNodeAssociation& GetAssociation()
			{
				return _association;
			}

#pragma region Relations
			Transform* GetParent()
			{
				return _parentTransform;
			}

			void SetParent(Transform* transform, bool worldPositionStays)
			{
#ifdef __PRE_DEBUG__
				if (transform == this)
				{
					throw "Cannot set Parent to self.";
				}
				else
#endif
				if (transform == _parentTransform)
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
					
					if (_parentTransform != nullptr)
					{
						_parentTransform->_childTransforms.push_back(this);
					}

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

			list<Transform*> GetChildren()
			{
				return _childTransforms;
			}
#pragma endregion

#pragma region Position
			const glm::vec3& GetPosition()
			{
				return _position;
			}

			const glm::vec3& GetLocalPosition()
			{
				return _localPosition;
			}

			void SetPosition(const glm::vec3& position)
			{
				_position = position;
				UpdateMatrix();
			}

			void SetLocalPosition(const glm::vec3& localPosition)
			{
				_localPosition = localPosition;
				UpdateLocalMatrix();
			}
#pragma endregion

#pragma region Rotation
			const glm::fquat& GetRotation()
			{
				return _rotation;
			}

			const glm::fquat& GetLocalRotation()
			{
				return _localRotation;
			}

			void SetRotation(const glm::fquat& rotation)
			{
				_rotation = rotation;
				_euler = glm::degrees(glm::eulerAngles(_rotation));
				UpdateMatrix();
			}

			void SetLocalRotation(const glm::fquat& localRotation)
			{
				_localRotation = localRotation;
				_localEuler = glm::degrees(glm::eulerAngles(_localRotation));
				UpdateLocalMatrix();
			}

			const glm::vec3& GetEuler()
			{
				return _euler;
			}

			const glm::vec3& GetLocalEuler()
			{
				return _localEuler;
			}

			void SetEuler(const glm::vec3& euler)
			{
				_euler = euler;
				_rotation = glm::quat(glm::radians(_euler));
				UpdateMatrix();
			}

			void SetLocalEuler(const glm::vec3& localEuler)
			{
				_localEuler = localEuler;
				_localRotation = glm::quat(glm::radians(_localEuler));
				UpdateLocalMatrix();
			}
#pragma endregion

#pragma region Scale
			const glm::vec3& GetScale()
			{
				return _scale;
			}

			const glm::vec3& GetLocalScale()
			{
				return _localScale;
			}

			void SetScale(const glm::vec3& scale)
			{
				_scale = scale;
				UpdateMatrix();
			}

			void SetLocalScale(const glm::vec3& localScale)
			{
				_localScale = localScale;
				UpdateLocalMatrix();
			}
#pragma endregion

			const glm::mat4& GetMatrix() const
			{
				return _matrix;
			}

			const glm::mat4& GetInverseMatrix() const
			{
				return _inverseMatrix;
			}

		private:
			TNodeAssociation& _association;

			Transform* _parentTransform;
			list<Transform*> _childTransforms;

			glm::mat4 _matrix;
			glm::mat4 _inverseMatrix;

			glm::mat4 _localMatrix; // local-to-parent

			glm::vec3 _position;
			glm::vec3 _localPosition;

			glm::fquat _rotation;
			glm::fquat _localRotation;

			glm::vec3 _euler;
			glm::vec3 _localEuler;

			glm::vec3 _scale;
			glm::vec3 _localScale;

			void UpdateMatrix()
			{
				_matrix = glm::translate(glm::mat4(), _position);
				_matrix = _matrix * glm::mat4_cast(_rotation);
				_matrix = glm::scale(_matrix, _scale);

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

			void UpdateLocalMatrix()
			{
				_localMatrix = glm::translate(glm::mat4(), _localPosition);
				_localMatrix = _localMatrix * glm::mat4_cast(_localRotation);
				_localMatrix = glm::scale(_localMatrix, _localScale);

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

			void AncestryChanged()
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
		};
	} // namespace TransformSubsystem
} // namespace PRE