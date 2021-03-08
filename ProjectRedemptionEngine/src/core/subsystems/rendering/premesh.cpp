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

		const glm::vec3* const PREMesh::GetVertices(unsigned int& nVertices) const
		{
			return _mesh.GetVertices(nVertices);
		}

		void PREMesh::SetNormals(const glm::vec3* const normals, unsigned int nNormals)
		{
			_mesh.SetNormals(normals, nNormals);
		}

		const glm::vec3* const PREMesh::GetNormals(unsigned int& nNormals) const
		{
			return _mesh.GetNormals(nNormals);
		}

		void PREMesh::SetUvs(const glm::vec2* const uvs, unsigned int nUvs)
		{
			_mesh.SetUvs(uvs, nUvs);
		}

		const glm::vec2* const PREMesh::GetUvs(unsigned int& nUvs) const
		{
			return _mesh.GetUvs(nUvs);
		}

		void PREMesh::SetBoneIds(const glm::ivec4* const boneIds, unsigned int nBoneIds)
		{
			_mesh.SetBoneIds(boneIds, nBoneIds);
		}

		const glm::ivec4* const PREMesh::GetBoneIds(unsigned int& nBoneIds) const
		{
			return _mesh.GetBoneIds(nBoneIds);
		}

		void PREMesh::SetBoneWeights(const glm::vec4* const boneWeights, unsigned int nBoneWeights)
		{
			_mesh.SetBoneWeights(boneWeights, nBoneWeights);
		}

		const glm::vec4* const PREMesh::GetBoneWeights(unsigned int& nBoneWeights) const
		{
			return _mesh.GetBoneWeights(nBoneWeights);
		}

		void PREMesh::SetTriangles(const unsigned int* const triangles, unsigned int nTriangles)
		{
			_mesh.SetTriangles(triangles, nTriangles);
		}

		const unsigned int* const PREMesh::GetTriangles(unsigned int& nTriangles) const
		{
			return _mesh.GetTriangles(nTriangles);
		}

		PREMesh::PREMesh(RenderMesh& mesh)
			:
			_mesh(mesh) {}

		PREMesh::~PREMesh() {}
	} // namespace Core
} // namespace PRE