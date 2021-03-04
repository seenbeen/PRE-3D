#include <core/subsystems/rendering/premesh.h>

#include <glm/glm.hpp>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		void PREMesh::SetVertices(const glm::vec3* const vertices, unsigned int nVertices)
		{
			_mesh.SetVertices(vertices, nVertices);
		}

		const glm::vec3* const PREMesh::GetVertices() const
		{
			return _mesh.GetVertices();
		}

		void PREMesh::SetNormals(const glm::vec3* const normals, unsigned int nNormals)
		{
			_mesh.SetNormals(normals, nNormals);
		}

		const glm::vec3* const PREMesh::GetNormals() const
		{
			return _mesh.GetNormals();
		}

		void PREMesh::SetUvs(const glm::vec2* const uvs, unsigned int nUvs)
		{
			_mesh.SetUvs(uvs, nUvs);
		}

		const glm::vec2* const PREMesh::GetUvs() const
		{
			return _mesh.GetUvs();
		}

		void PREMesh::SetTriangles(const unsigned int* const triangles, unsigned int nTriangles)
		{
			_mesh.SetTriangles(triangles, nTriangles);
		}

		const unsigned int* const PREMesh::GetTriangles() const
		{
			return _mesh.GetTriangles();
		}

		PREMesh::PREMesh(RenderMesh& mesh)
			:
			_mesh(mesh) {}

		PREMesh::~PREMesh() {}
	} // namespace Core
} // namespace PRE