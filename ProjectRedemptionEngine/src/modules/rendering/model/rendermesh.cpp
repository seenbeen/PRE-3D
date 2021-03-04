#include <modules/rendering/model/rendermesh.h>

#include <cstring>

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		RenderMesh::Impl& RenderMesh::Impl::MakeImpl()
		{
			GLuint vertexArrayObject;
			GLuint 	vertexVerticesBuffer;
			GLuint 	vertexNormalsBuffer;
			GLuint 	vertexUVsBuffer;
			GLuint 	elementsTrianglesBuffer;
			
			glGenVertexArrays(1, &vertexArrayObject);
			glGenBuffers(1, &vertexVerticesBuffer);
			glGenBuffers(1, &vertexNormalsBuffer);
			glGenBuffers(1, &vertexUVsBuffer);
			glGenBuffers(1, &elementsTrianglesBuffer);

			glBindVertexArray(vertexArrayObject);

			glBindBuffer(GL_ARRAY_BUFFER, vertexVerticesBuffer);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, sizeof(glm::vec3), GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, vertexNormalsBuffer);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, sizeof(glm::vec3), GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, vertexUVsBuffer);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, sizeof(glm::vec2), GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

			glBindVertexArray(0);

			return *(new Impl(
				vertexArrayObject,
				vertexVerticesBuffer,
				vertexNormalsBuffer,
				vertexUVsBuffer,
				elementsTrianglesBuffer
			));
		}

		RenderMesh::Impl::Impl(
			GLuint vertexArrayObject,
			GLuint vertexVerticesBuffer,
			GLuint vertexNormalsBuffer,
			GLuint vertexUVsBuffer,
			GLuint elementsTrianglesBuffer
		)
			:
			vertexArrayObject(vertexArrayObject),
			vertexVerticesBuffer(vertexVerticesBuffer),
			vertexNormalsBuffer(vertexNormalsBuffer),
			vertexUVsBuffer(vertexUVsBuffer),
			elementsTrianglesBuffer(elementsTrianglesBuffer),
			nVertices(0),
			vertices(nullptr),
			nNormals(0),
			normals(nullptr),
			nUvs(0),
			uvs(nullptr),
			nTriangleIndices(0),
			triangleIndices(nullptr),
			verticesHaveChanged(false),
			normalsHaveChanged(false),
			uvsHaveChanged(false),
			triangleIndicesHaveChanged(false) {}

		RenderMesh::Impl::~Impl()
		{
			if (triangleIndices != nullptr)
			{
				delete[] triangleIndices;
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
			glDeleteBuffers(1, &vertexUVsBuffer);
			glDeleteBuffers(1, &vertexNormalsBuffer);
			glDeleteBuffers(1, &vertexVerticesBuffer);
			glDeleteVertexArrays(1, &vertexArrayObject);
		}

		void RenderMesh::SetVertices(const glm::vec3* const vertices, unsigned int nVertices)
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

		const glm::vec3* const RenderMesh::GetVertices() const
		{
			return _impl.vertices;
		}

		void RenderMesh::SetNormals(const glm::vec3* const normals, unsigned int nNormals)
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

		const glm::vec3* const RenderMesh::GetNormals() const
		{
			return _impl.normals;
		}

		void RenderMesh::SetUvs(const glm::vec2* const uvs, unsigned int nuvs)
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

		const glm::vec2* const RenderMesh::GetUvs() const
		{
			return _impl.uvs;
		}

		void RenderMesh::SetTriangles(const unsigned int* const triangles, unsigned int nTriangleIndices)
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

		const unsigned int* const RenderMesh::GetTriangles() const
		{
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
				glBindBuffer(GL_ARRAY_BUFFER, _impl.vertexVerticesBuffer);
				glBufferData(GL_ARRAY_BUFFER, _impl.nNormals * sizeof(glm::vec3), &_impl.normals[0], GL_STATIC_DRAW);
			}
			if (_impl.uvsHaveChanged)
			{
				_impl.uvsHaveChanged = false;
				glBindBuffer(GL_ARRAY_BUFFER, _impl.vertexVerticesBuffer);
				glBufferData(GL_ARRAY_BUFFER, _impl.nUvs * sizeof(glm::vec2), &_impl.uvs[0], GL_STATIC_DRAW);
			}
			if (_impl.triangleIndicesHaveChanged)
			{
				_impl.triangleIndicesHaveChanged = false;
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _impl.elementsTrianglesBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, _impl.nTriangleIndices * sizeof(unsigned int), &_impl.triangleIndices[0], GL_STATIC_DRAW);
			}
			glDrawElements(GL_TRIANGLES, _impl.nTriangleIndices, GL_UNSIGNED_INT, 0);
			glBindVertexArray(GL_NONE);
		}
	} // namespace RenderingModule
} // namespace PRE