#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class RenderModel;
		class Renderer;

		// TODO: bone-channels, weights
		class RenderMesh
		{
			RenderMesh& operator=(const RenderMesh&) = delete;
			RenderMesh(const RenderMesh&) = delete;

			friend class RenderModel;
			friend class Renderer;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderMesh;

			private:
				const GLuint vertexArrayObject;

				const GLuint vertexVerticesBuffer;
				const GLuint vertexNormalsBuffer;
				const GLuint vertexUVsBuffer;

				const GLuint elementsTrianglesBuffer;

				unsigned int nVertices;
				glm::vec3* vertices;

				unsigned int nNormals;
				glm::vec3* normals;

				unsigned int nUvs;
				glm::vec2* uvs;

				unsigned int nTriangleIndices;
				unsigned int* triangleIndices;

				bool verticesHaveChanged;
				bool normalsHaveChanged;
				bool uvsHaveChanged;

				bool triangleIndicesHaveChanged;

				static Impl& MakeImpl();
				Impl(
					GLuint vertexArrayObject,
					GLuint vertexVerticesBuffer,
					GLuint vertexNormalsBuffer,
					GLuint vertexUVsBuffer,
					GLuint elementsTrianglesBuffer
				);
				~Impl();
			};

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
			Impl& _impl;

			RenderMesh();
			~RenderMesh();

			void Render();
		};
	} // namespace RenderingModule
} // namespace PRE