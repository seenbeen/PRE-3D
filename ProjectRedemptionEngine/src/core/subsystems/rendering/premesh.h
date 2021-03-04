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
			const glm::vec3* const GetVertices() const;

			void SetNormals(const glm::vec3* const normals, unsigned int nNormals);
			const glm::vec3* const GetNormals() const;

			void SetUvs(const glm::vec2* const uvs, unsigned int nUvs);
			const glm::vec2* const GetUvs() const;

			void SetTriangles(const unsigned int* const triangles, unsigned int nTriangles);
			const unsigned int* const GetTriangles() const;

		private:
			RenderMesh& _mesh;

			PREMesh(RenderMesh& mesh);
			~PREMesh();
		};
	} // namespace Core
} // namespace PRE