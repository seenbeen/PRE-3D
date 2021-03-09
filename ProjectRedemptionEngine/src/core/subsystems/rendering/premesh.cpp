#include <core/subsystems/rendering/premesh.h>

#include <glm/glm.hpp>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		void PREMesh::SetRenderedTriangles(
			unsigned int startTriangleIndex,
			unsigned int endTriangleIndex
		)
		{
			_mesh.SetRenderedTriangles(
				startTriangleIndex,
				endTriangleIndex
			);
		}

		PREMesh::PREMesh(RenderMesh& mesh)
			:
			_mesh(mesh) {}

		PREMesh::~PREMesh() {}
	} // namespace Core
} // namespace PRE