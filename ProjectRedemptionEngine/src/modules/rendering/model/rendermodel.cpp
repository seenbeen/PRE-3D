#include <modules/rendering/model/rendermodel.h>

#include <modules/rendering/model/rendermesh.h>
#include <modules/rendering/model/rendermaterial.h>

namespace PRE
{
	namespace RenderingModule
	{
		RenderModel::Impl& RenderModel::Impl::MakeImpl()
		{
			return *(new Impl());
		}

		RenderModel::Impl::Impl()
			:
			pMesh(nullptr),
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

		void RenderModel::SetMaterial(RenderMaterial* pMaterial)
		{
			_impl.pMaterial = pMaterial;
		}

		void RenderModel::Render(const glm::mat4& viewProjectionMatrix)
		{
			if (_impl.pMesh != nullptr && _impl.pMaterial != nullptr)
			{
				_impl.pMaterial->Bind(viewProjectionMatrix * modelMatrix);
				_impl.pMesh->Render();
			}
		}
	} // namespace RenderingModule
} // namespace PRE