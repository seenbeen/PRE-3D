#include <modules/rendering/model/rendermesh.h>

#include <cstring>
#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		RenderMesh::Impl& RenderMesh::Impl::MakeImpl()
		{
			GLuint vertexArrayObject;
			GLuint vertexVerticesBuffer;
			GLuint vertexNormalsBuffer;
			GLuint vertexUVsBuffer;
			GLuint vertexBoneIdsBuffer;
			GLuint vertexBoneWeightsBuffer;
			GLuint elementsTrianglesBuffer;
			
			glGenVertexArrays(1, &vertexArrayObject);
			glGenBuffers(1, &vertexVerticesBuffer);
			glGenBuffers(1, &vertexNormalsBuffer);
			glGenBuffers(1, &vertexUVsBuffer);
			glGenBuffers(1, &vertexBoneIdsBuffer);
			glGenBuffers(1, &vertexBoneWeightsBuffer);
			glGenBuffers(1, &elementsTrianglesBuffer);

			glBindVertexArray(vertexArrayObject);

			glBindBuffer(GL_ARRAY_BUFFER, vertexVerticesBuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, vertexNormalsBuffer);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, vertexUVsBuffer);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBoneIdsBuffer);
			glVertexAttribIPointer(3, 4, GL_INT, 0, (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBoneWeightsBuffer);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(4);

			glBindVertexArray(0);

			return *(new Impl(
				vertexArrayObject,
				vertexVerticesBuffer,
				vertexNormalsBuffer,
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
			vertexBoneIdsBuffer(vertexBoneIdsBuffer),
			vertexBoneWeightsBuffer(vertexBoneWeightsBuffer),
			elementsTrianglesBuffer(elementsTrianglesBuffer),
			nVertices(0),
			vertices(nullptr),
			nNormals(0),
			normals(nullptr),
			nUvs(0),
			uvs(nullptr),
			nBoneIds(0),
			boneIds(nullptr),
			nBoneWeights(0),
			boneWeights(nullptr),
			nTriangleIndices(0),
			triangleIndices(nullptr),
			verticesHaveChanged(false),
			normalsHaveChanged(false),
			uvsHaveChanged(false),
			boneIdsHaveChanged(false),
			boneWeightsHaveChanged(false),
			triangleIndicesHaveChanged(false) {}

		RenderMesh::Impl::~Impl()
		{
			if (triangleIndices != nullptr)
			{
				delete[] triangleIndices;
			}
			if (boneWeights != nullptr)
			{
				delete[] boneWeights;
			}
			if (boneIds != nullptr)
			{
				delete[] boneIds;
			}
			if (uvs != nullptr)
			{
				delete[] uvs;
			}
			if (normals != nullptr)
			{
				delete[] normals;
			}
			if (vertices != nullptr)
			{
				delete[] vertices;
			}

			glDeleteBuffers(1, &elementsTrianglesBuffer);
			glDeleteBuffers(1, &vertexBoneWeightsBuffer);
			glDeleteBuffers(1, &vertexBoneIdsBuffer);
			glDeleteBuffers(1, &vertexUVsBuffer);
			glDeleteBuffers(1, &vertexNormalsBuffer);
			glDeleteBuffers(1, &vertexVerticesBuffer);
			glDeleteVertexArrays(1, &vertexArrayObject);
		}

		void RenderMesh::SetVertices(const glm::vec3* vertices, unsigned int nVertices)
		{
			if (_impl.vertices != nullptr)
			{
				delete[] _impl.vertices;
			}
			if (vertices != nullptr && nVertices != 0)
			{
				_impl.nVertices = nVertices;
				_impl.vertices = new glm::vec3[_impl.nVertices];
				std::memcpy(_impl.vertices, vertices, _impl.nVertices * sizeof(glm::vec3));
			}
			else
			{
				_impl.nVertices = 0;
				_impl.vertices = nullptr;
			}
			_impl.verticesHaveChanged = true;
		}

		const glm::vec3* const RenderMesh::GetVertices(unsigned int& nVertices) const
		{
			nVertices = _impl.nVertices;
			return _impl.vertices;
		}

		void RenderMesh::SetNormals(const glm::vec3* normals, unsigned int nNormals)
		{
			if (_impl.normals != nullptr)
			{
				delete[] _impl.normals;
			}
			if (normals != nullptr && nNormals != 0)
			{
				_impl.nNormals = nNormals;
				_impl.normals = new glm::vec3[_impl.nNormals];
				std::memcpy(_impl.normals, normals, _impl.nNormals * sizeof(glm::vec3));
			}
			else
			{
				_impl.nNormals = 0;
				_impl.normals = nullptr;
			}
			_impl.normalsHaveChanged = true;
		}

		const glm::vec3* const RenderMesh::GetNormals(unsigned int& nNormals) const
		{
			nNormals = _impl.nNormals;
			return _impl.normals;
		}

		void RenderMesh::SetUvs(const glm::vec2* uvs, unsigned int nuvs)
		{
			if (_impl.uvs != nullptr)
			{
				delete[] _impl.uvs;
			}
			if (uvs != nullptr && nuvs != 0)
			{
				_impl.nUvs = nuvs;
				_impl.uvs = new glm::vec2[_impl.nUvs];
				std::memcpy(_impl.uvs, uvs, _impl.nUvs * sizeof(glm::vec2));
			}
			else
			{
				_impl.nUvs = 0;
				_impl.uvs = nullptr;
			}
			_impl.uvsHaveChanged = true;
		}

		const glm::vec2* const RenderMesh::GetUvs(unsigned int& nUvs) const
		{
			nUvs = _impl.nUvs;
			return _impl.uvs;
		}

		void RenderMesh::SetBoneIds(const glm::ivec4* boneIds, unsigned int nBoneIds)
		{
			if (_impl.boneIds != nullptr)
			{
				delete[] _impl.boneIds;
			}
			if (boneIds != nullptr && nBoneIds != 0)
			{
				_impl.nBoneIds = nBoneIds;
				_impl.boneIds = new glm::ivec4[_impl.nBoneIds];
				std::memcpy(_impl.boneIds, boneIds, _impl.nBoneIds * sizeof(glm::ivec4));
			}
			else
			{
				_impl.nBoneIds = 0;
				_impl.boneIds = nullptr;
			}
			_impl.boneIdsHaveChanged = true;
		}

		const glm::ivec4* const RenderMesh::GetBoneIds(unsigned int& nBoneIds) const
		{
			nBoneIds = _impl.nBoneIds;
			return _impl.boneIds;
		}

		void RenderMesh::SetBoneWeights(const glm::vec4* boneWeights, unsigned int nBoneWeights)
		{
			if (_impl.boneWeights != nullptr)
			{
				delete[] _impl.boneWeights;
			}
			if (boneWeights != nullptr && nBoneWeights != 0)
			{
				_impl.nBoneWeights = nBoneWeights;
				_impl.boneWeights = new glm::vec4[_impl.nBoneWeights];
				std::memcpy(_impl.boneWeights, boneWeights, _impl.nBoneWeights * sizeof(glm::vec4));
			}
			else
			{
				_impl.nBoneWeights = 0;
				_impl.boneWeights = nullptr;
			}
			_impl.boneWeightsHaveChanged = true;
		}

		const glm::vec4* const RenderMesh::GetBoneWeights(unsigned int& nBoneWeights) const
		{
			nBoneWeights = _impl.nBoneWeights;
			return _impl.boneWeights;
		}

		void RenderMesh::SetTriangles(const unsigned int* triangles, unsigned int nTriangleIndices)
		{
			if (_impl.triangleIndices != nullptr)
			{
				delete[] _impl.triangleIndices;
			}
			if (triangles != nullptr && nTriangleIndices != 0)
			{
				_impl.nTriangleIndices = nTriangleIndices;
				_impl.triangleIndices = new unsigned int[_impl.nTriangleIndices];
				
				std::memcpy(_impl.triangleIndices, triangles, _impl.nTriangleIndices * sizeof(unsigned int));
			}
			else
			{
				_impl.nTriangleIndices = 0;
				_impl.triangleIndices = nullptr;
			}
			_impl.triangleIndicesHaveChanged = true;
		}

		const unsigned int* const RenderMesh::GetTriangles(unsigned int& nTriangles) const
		{
			nTriangles = _impl.nTriangleIndices;
			return _impl.triangleIndices;
		}

		RenderMesh::RenderMesh()
			: _impl(Impl::MakeImpl()) {}

		RenderMesh::~RenderMesh()
		{
			delete &_impl;
		}

		void RenderMesh::Render()
		{
			if (
				_impl.nVertices == 0 ||
				_impl.nNormals == 0 ||
				_impl.nUvs == 0 ||
				_impl.nBoneIds == 0 ||
				_impl.nBoneWeights == 0 ||
				_impl.nTriangleIndices == 0
			)
			{
				return;
			}

			glBindVertexArray(_impl.vertexArrayObject);

			if (_impl.verticesHaveChanged)
			{
				_impl.verticesHaveChanged = false;
				glBindBuffer(GL_ARRAY_BUFFER, _impl.vertexVerticesBuffer);
				glBufferData(GL_ARRAY_BUFFER, _impl.nVertices * sizeof(glm::vec3), &_impl.vertices[0], GL_STATIC_DRAW);
			}
			if (_impl.normalsHaveChanged)
			{
				_impl.normalsHaveChanged = false;
				glBindBuffer(GL_ARRAY_BUFFER, _impl.vertexNormalsBuffer);
				glBufferData(GL_ARRAY_BUFFER, _impl.nNormals * sizeof(glm::vec3), &_impl.normals[0], GL_STATIC_DRAW);
			}
			if (_impl.uvsHaveChanged)
			{
				_impl.uvsHaveChanged = false;
				glBindBuffer(GL_ARRAY_BUFFER, _impl.vertexUVsBuffer);
				glBufferData(GL_ARRAY_BUFFER, _impl.nUvs * sizeof(glm::vec2), &_impl.uvs[0], GL_STATIC_DRAW);
			}

			if (_impl.boneIdsHaveChanged)
			{
				_impl.boneIdsHaveChanged = false;
				glBindBuffer(GL_ARRAY_BUFFER, _impl.vertexBoneIdsBuffer);
				glBufferData(GL_ARRAY_BUFFER, _impl.nBoneIds * sizeof(glm::ivec4), &_impl.boneIds[0], GL_STATIC_DRAW);
			}
			if (_impl.boneWeightsHaveChanged)
			{
				_impl.boneWeightsHaveChanged = false;
				glBindBuffer(GL_ARRAY_BUFFER, _impl.vertexBoneWeightsBuffer);
				glBufferData(GL_ARRAY_BUFFER, _impl.nBoneWeights * sizeof(glm::vec4), &_impl.boneWeights[0], GL_STATIC_DRAW);
			}

			if (_impl.triangleIndicesHaveChanged)
			{
				_impl.triangleIndicesHaveChanged = false;
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _impl.elementsTrianglesBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, _impl.nTriangleIndices * sizeof(unsigned int), _impl.triangleIndices, GL_STATIC_DRAW);
			}

			glDrawElements(GL_TRIANGLES, _impl.nTriangleIndices, GL_UNSIGNED_INT, 0);

			glBindVertexArray(GL_NONE);
		}
	} // namespace RenderingModule
} // namespace PRE