#include <modules/rendering/model/rendermodel.h>

#include <modules/rendering/model/rendermesh.h>
#include <modules/rendering/model/rendermaterial.h>

namespace PRE
{
	namespace RenderingModule
	{
		RenderModel::Impl& RenderModel::Impl::MakeImpl(
			RenderMesh& mesh,
			RenderMaterial& material
		)
		{
			return *(new Impl(mesh, material));
		}

		RenderModel::Impl::Impl(RenderMesh& mesh, RenderMaterial& material)
			:
			material(&material),
			mesh(&mesh) {}

		RenderModel::Impl::~Impl() {}

		RenderModel::RenderModel(
			const unsigned int renderTag,
			RenderMesh& mesh,
			RenderMaterial& material
		)
			:
			_impl(Impl::MakeImpl(mesh, material)),
			_renderTag(renderTag),
			modelMatrix() {}

		RenderModel::~RenderModel()
		{
			delete &_impl;
		}

		void RenderModel::SetMesh(RenderMesh& mesh)
		{
			_impl.mesh = &mesh;
		}

		void RenderModel::SetMaterial(RenderMaterial& material)
		{
			_impl.material = &material;
		}

		void RenderModel::Render(const glm::mat4& viewProjectionMatrix)
		{
			_impl.material->Bind(viewProjectionMatrix * modelMatrix);
			_impl.mesh->Render();
		}
	} // namespace RenderingModule
} // namespace PRE