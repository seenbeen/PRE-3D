#pragma once
#include <glm/glm.hpp>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;

		using PRE::RenderingModule::RenderMesh;

		class PREMesh
		{
			PREMesh& operator=(const PREMesh&) = delete;
			PREMesh(const PREMesh&) = delete;

			friend class PRERendering;

		public:
			void SetVertices(const glm::vec3* const vertices, unsigned int nVertices);
			const glm::vec3* const GetVertices(unsigned int& nVertices) const;

			void SetNormals(const glm::vec3* const normals, unsigned int nNormals);
			const glm::vec3* const GetNormals(unsigned int& nNormals) const;

			void SetUvs(const glm::vec2* const uvs, unsigned int nUvs);
			const glm::vec2* const GetUvs(unsigned int& nUvs) const;

			void SetVertexBoneIds(const glm::ivec4* const vertexBoneIds, unsigned int nVertexBoneIds);
			const glm::ivec4* const GetVertexBoneIds(unsigned int& nVertexBoneIds) const;

			void SetVertexBoneWeights(const glm::vec4* const vertexBoneWeights, unsigned int nVertexBoneWeights);
			const glm::vec4* const GetVertexBoneWeights(unsigned int& nVertexBoneWeights) const;

			void SetTriangles(const unsigned int* const triangles, unsigned int nTriangles);
			const unsigned int* const GetTriangles(unsigned int& nTriangles) const;

		private:
			RenderMesh& _mesh;

			PREMesh(RenderMesh& mesh);
			~PREMesh();
		};
	} // namespace Core
} // namespace PRE