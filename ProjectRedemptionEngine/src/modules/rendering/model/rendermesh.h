#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderModel;

		class RenderMesh
		{
			RenderMesh& operator=(const RenderMesh&) = delete;
			RenderMesh(const RenderMesh&) = delete;

			friend class Renderer;
			friend class RenderModel;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderMesh;

			private:
				const GLuint vertexArrayObject;

				// TODO: Optimize into array -> single call to glBuffers
				const GLuint vertexVerticesBuffer;
				const GLuint vertexNormalsBuffer;
				const GLuint vertexTangentsBuffer;
				const GLuint vertexBiTangentsBuffer;
				const GLuint vertexUVsBuffer;
				const GLuint vertexBoneIdsBuffer;
				const GLuint vertexBoneWeightsBuffer;

				const GLuint elementsTrianglesBuffer;

				unsigned int elementsDrawStartIndex;
				unsigned int elementsDrawCount;

				static Impl& MakeImpl(
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
				);

				Impl(
					GLuint vertexArrayObject,
					GLuint vertexVerticesBuffer,
					GLuint vertexNormalsBuffer,
					GLuint vertexTangentsBuffer,
					GLuint vertexBiTangentsBuffer,
					GLuint vertexUVsBuffer,
					GLuint vertexBoneIdsBuffer,
					GLuint vertexWeightsBuffer,
					GLuint elementsTrianglesBuffer
				);
				~Impl();
			};

		public:
			// used for spriting
			void SetRenderedTriangles(unsigned int startTriangleIndex, unsigned int endTriangleIndex);

		private:
			Impl& _impl;

			// TODO: instead of passing pointers, pass ref to gl buffer?
			// basically an asset management overhaul

			RenderMesh(
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
			);

			~RenderMesh();

			void Render();
		};
	} // namespace RenderingModule
} // namespace PRE