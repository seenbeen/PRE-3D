#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderModel;

		// TODO: bone-channels, weights
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

				const GLuint vertexVerticesBuffer;
				const GLuint vertexNormalsBuffer;
				const GLuint vertexUVsBuffer;
				const GLuint vertexBoneIdsBuffer;
				const GLuint vertexBoneWeightsBuffer;

				const GLuint elementsTrianglesBuffer;

				unsigned int nVertices;
				glm::vec3* vertices;

				unsigned int nNormals;
				glm::vec3* normals;

				unsigned int nUvs;
				glm::vec2* uvs;

				unsigned int nBoneIds;
				glm::ivec4* boneIds;

				unsigned int nBoneWeights;
				glm::vec4* boneWeights;

				unsigned int nTriangleIndices;
				unsigned int* triangleIndices;

				bool verticesHaveChanged;
				bool normalsHaveChanged;
				bool uvsHaveChanged;

				bool boneIdsHaveChanged;
				bool boneWeightsHaveChanged;

				bool triangleIndicesHaveChanged;

				static Impl& MakeImpl();
				Impl(
					GLuint vertexArrayObject,
					GLuint vertexVerticesBuffer,
					GLuint vertexNormalsBuffer,
					GLuint vertexUVsBuffer,
					GLuint vertexBoneIdsBuffer,
					GLuint vertexWeightsBuffer,
					GLuint elementsTrianglesBuffer
				);
				~Impl();
			};

		public:
			void SetVertices(const glm::vec3* vertices, unsigned int nVertices);
			const glm::vec3* const GetVertices(unsigned int& nVertices) const;

			void SetNormals(const glm::vec3* normals, unsigned int nNormals);
			const glm::vec3* const GetNormals(unsigned int& nNormals) const;

			void SetUvs(const glm::vec2* uvs, unsigned int nUvs);
			const glm::vec2* const GetUvs(unsigned int& nUvs) const;

			void SetBoneIds(const glm::ivec4* boneIds, unsigned int nBoneIds);
			const glm::ivec4* const GetBoneIds(unsigned int& nBoneIds) const;

			void SetBoneWeights(const glm::vec4* boneWeights, unsigned int nBoneWeights);
			const glm::vec4* const GetBoneWeights(unsigned int& nBoneWeights) const;

			void SetTriangles(const unsigned int* const triangles, unsigned int nTriangles);
			const unsigned int* const GetTriangles(unsigned int& nTriangles) const;

		private:
			Impl& _impl;

			RenderMesh();
			~RenderMesh();

			void Render();
		};
	} // namespace RenderingModule
} // namespace PRE