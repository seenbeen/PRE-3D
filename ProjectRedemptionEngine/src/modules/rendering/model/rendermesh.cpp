#include <modules/rendering/model/rendermesh.h>

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		using std::vector;

		RenderMesh::Impl& RenderMesh::Impl::MakeImpl(
			unsigned int nVertices,
			const glm::vec3* vertices,
			const glm::vec3* normals,
			const glm::vec3* tangents,
			const glm::vec3* biTangents,
			const glm::vec2* uvs,
			const glm::ivec4* vertexBoneIds,
			const glm::vec4* vertexBoneWeights,
			unsigned int nTriangleElements,
			const unsigned int* const triangleElements
		)
		{
			auto CAPACITY_VEC2 = nVertices * sizeof(glm::vec2);
			auto CAPACITY_VEC3 = nVertices * sizeof(glm::vec3);
			auto CAPACITY_VEC4 = nVertices * sizeof(glm::vec4);
			auto CAPACITY_I_VEC4 = nVertices * sizeof(glm::ivec4);
			auto CAPACITY_TRI = nTriangleElements * sizeof(unsigned int);

			vector<glm::vec2> fillVec2(nVertices, glm::vec2());
			vector<glm::vec3> fillVec3(nVertices, glm::vec3());
			vector<glm::vec4> fillVec4(nVertices, glm::vec4());
			vector<glm::ivec4> fillIVec4(nVertices, glm::ivec4());

			if (normals == nullptr)
			{
				normals = &fillVec3[0];
			}

			if (tangents == nullptr)
			{
				tangents = &fillVec3[0];
			}

			if (biTangents == nullptr)
			{
				biTangents = &fillVec3[0];
			}

			if (uvs == nullptr)
			{
				uvs = &fillVec2[0];
			}

			if (vertexBoneIds == nullptr)
			{
				vertexBoneIds = &fillIVec4[0];
			}

			if (vertexBoneWeights == nullptr)
			{
				vertexBoneWeights = &fillVec4[0];
			}

			GLuint vertexArrayObject;
			GLuint vertexVerticesBuffer;
			GLuint vertexNormalsBuffer;
			GLuint vertexTangentsBuffer;
			GLuint vertexBiTangentsBuffer;
			GLuint vertexUVsBuffer;
			GLuint vertexBoneIdsBuffer;
			GLuint vertexBoneWeightsBuffer;
			GLuint elementsTrianglesBuffer;

			glGenVertexArrays(1, &vertexArrayObject);
			glGenBuffers(1, &vertexVerticesBuffer);
			glGenBuffers(1, &vertexNormalsBuffer);
			glGenBuffers(1, &vertexTangentsBuffer);
			glGenBuffers(1, &vertexBiTangentsBuffer);
			glGenBuffers(1, &vertexUVsBuffer);
			glGenBuffers(1, &vertexBoneIdsBuffer);
			glGenBuffers(1, &vertexBoneWeightsBuffer);
			glGenBuffers(1, &elementsTrianglesBuffer);

			glBindVertexArray(vertexArrayObject);

			glBindBuffer(GL_ARRAY_BUFFER, vertexVerticesBuffer);
			glBufferData(GL_ARRAY_BUFFER, CAPACITY_VEC3, vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, vertexNormalsBuffer);
			glBufferData(GL_ARRAY_BUFFER, CAPACITY_VEC3, normals, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, vertexTangentsBuffer);
			glBufferData(GL_ARRAY_BUFFER, CAPACITY_VEC3, tangents, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBiTangentsBuffer);
			glBufferData(GL_ARRAY_BUFFER, CAPACITY_VEC3, biTangents, GL_STATIC_DRAW);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, vertexUVsBuffer);
			glBufferData(GL_ARRAY_BUFFER, CAPACITY_VEC2, uvs, GL_STATIC_DRAW);
			glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBoneIdsBuffer);
			glBufferData(GL_ARRAY_BUFFER, CAPACITY_I_VEC4, vertexBoneIds, GL_STATIC_DRAW);
			glVertexAttribIPointer(5, 4, GL_INT, 0, (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBoneWeightsBuffer);
			glBufferData(GL_ARRAY_BUFFER, CAPACITY_VEC4, vertexBoneWeights, GL_STATIC_DRAW);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsTrianglesBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, CAPACITY_TRI, triangleElements, GL_STATIC_DRAW);

			glBindVertexArray(0);

			return *(new Impl(
				vertexArrayObject,
				vertexVerticesBuffer,
				vertexNormalsBuffer,
				vertexTangentsBuffer,
				vertexBiTangentsBuffer,
				vertexUVsBuffer,
				vertexBoneIdsBuffer,
				vertexBoneWeightsBuffer,
				elementsTrianglesBuffer
			));
		}

		RenderMesh::Impl::Impl(
			GLuint vertexArrayObject,
			GLuint vertexVerticesBuffer,
			GLuint vertexNormalsBuffer,
			GLuint vertexTangentsBuffer,
			GLuint vertexBiTangentsBuffer,
			GLuint vertexUVsBuffer,
			GLuint vertexBoneIdsBuffer,
			GLuint vertexBoneWeightsBuffer,
			GLuint elementsTrianglesBuffer
		)
			:
			vertexArrayObject(vertexArrayObject),
			vertexVerticesBuffer(vertexVerticesBuffer),
			vertexNormalsBuffer(vertexNormalsBuffer),
			vertexUVsBuffer(vertexUVsBuffer),
			vertexTangentsBuffer(vertexTangentsBuffer),
			vertexBiTangentsBuffer(vertexBiTangentsBuffer),
			vertexBoneIdsBuffer(vertexBoneIdsBuffer),
			vertexBoneWeightsBuffer(vertexBoneWeightsBuffer),
			elementsTrianglesBuffer(elementsTrianglesBuffer),
			elementsDrawStartIndex(0),
			elementsDrawCount(0) {}

		RenderMesh::Impl::~Impl()
		{
			glDeleteBuffers(1, &elementsTrianglesBuffer);
			glDeleteBuffers(1, &vertexBoneWeightsBuffer);
			glDeleteBuffers(1, &vertexBoneIdsBuffer);
			glDeleteBuffers(1, &vertexUVsBuffer);
			glDeleteBuffers(1, &vertexBiTangentsBuffer);
			glDeleteBuffers(1, &vertexTangentsBuffer);
			glDeleteBuffers(1, &vertexNormalsBuffer);
			glDeleteBuffers(1, &vertexVerticesBuffer);
			glDeleteVertexArrays(1, &vertexArrayObject);
		}

		void RenderMesh::SetRenderedTriangles(
			unsigned int startTriangleIndex,
			unsigned int endTriangleIndex
		)
		{
			_impl.elementsDrawStartIndex = startTriangleIndex * 3;
			_impl.elementsDrawCount = (endTriangleIndex - startTriangleIndex) * 3;
		}

		RenderMesh::RenderMesh(
			unsigned int nVertices,
			const glm::vec3* vertices,
			const glm::vec3* normals,
			const glm::vec3* tangents,
			const glm::vec3* biTangents,
			const glm::vec2* uvs,
			const glm::ivec4* vertexBoneIds,
			const glm::vec4* vertexBoneWeights,
			unsigned int nTriangleElements,
			const unsigned int* const triangleElements
		)
			:
			_impl(Impl::MakeImpl(
				nVertices,
				vertices,
				normals,
				tangents,
				biTangents,
				uvs,
				vertexBoneIds,
				vertexBoneWeights,
				nTriangleElements,
				triangleElements
			))
		{
			SetRenderedTriangles(0, nTriangleElements / 3);
		}

		RenderMesh::~RenderMesh()
		{
			delete &_impl;
		}

		void RenderMesh::Render()
		{
			glBindVertexArray(_impl.vertexArrayObject);

			glDrawElements(
				GL_TRIANGLES,
				_impl.elementsDrawCount,
				GL_UNSIGNED_INT,
				(void*)_impl.elementsDrawStartIndex
			);

			glBindVertexArray(GL_NONE);
		}
	} // namespace RenderingModule
} // namespace PRE