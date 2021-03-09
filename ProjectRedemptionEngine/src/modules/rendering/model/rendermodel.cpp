#include <modules/rendering/model/rendermodel.h>

#include <vector>

#include <glm/glm.hpp>

#include <modules/rendering/model/rendermesh.h>
#include <modules/rendering/model/renderskeleton.h>
#include <modules/rendering/model/rendermaterial.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::vector;

		RenderModel::Impl& RenderModel::Impl::MakeImpl()
		{
			return *(new Impl());
		}

		RenderModel::Impl::Impl()
			:
			pMesh(nullptr),
			pSkeleton(nullptr),
			pMaterial(nullptr) {}

		RenderModel::Impl::~Impl() {}

		RenderModel::RenderModel()
			:
			_impl(Impl::MakeImpl()),
			modelMatrix() {}

		RenderModel::~RenderModel()
		{
			delete &_impl;
		}

		void RenderModel::SetMesh(RenderMesh* pMesh)
		{
			_impl.pMesh = pMesh;
		}

		void RenderModel::SetSkeleton(RenderSkeleton* pSkeleton)
		{
			_impl.pSkeleton = pSkeleton;
		}

		void RenderModel::SetMaterial(RenderMaterial* pMaterial)
		{
			_impl.pMaterial = pMaterial;
		}

		void RenderModel::Render(const glm::mat4& viewProjectionMatrix)
		{
			if (_impl.pMesh != nullptr && _impl.pMaterial != nullptr)
			{
				if (_impl.pSkeleton != nullptr)
				{
					// TODO: Cache allocated result memory?
					vector<glm::mat4> result;
					_impl.pSkeleton->GetCurrentState(result);
					_impl.pMaterial->Bind(
						viewProjectionMatrix * modelMatrix,
						&result
					);
				}
				else
				{
					_impl.pMaterial->Bind(
						viewProjectionMatrix * modelMatrix,
						nullptr
					);
				}
				_impl.pMesh->Render();
			}
		}
	} // namespace RenderingModule
} // namespace PRE