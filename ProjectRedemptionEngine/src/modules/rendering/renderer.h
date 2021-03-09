#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// TODO: migrate SDL to its own namespace to reduce global clutter
struct SDL_Window;
typedef void* SDL_GLContext;

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <modules/rendering/model/renderskeleton.h>

namespace PRE
{
	namespace RenderingModule
	{
		class RenderCompositingNode;
		class RenderCamera;
		class RenderVertexShader;
		class RenderFragmentShader;
		class RenderShaderProgram;
		class RenderMesh;
		class RenderTexture;
		class RenderMaterial;
		class RenderModel;

		using std::string;
		using std::unordered_map;
		using std::unordered_set;
		using std::vector;

		class Renderer
		{
			Renderer& operator=(const Renderer&) = delete;
			Renderer(const Renderer&) = delete;

		public:
			enum class CameraKind { ORTHOGRAPHIC, PERSPECTIVE };

			static const unsigned int ROOT_TAG_GROUP;

			static Renderer& MakeRenderer(
				const string& windowTitle,
				unsigned int windowWidth,
				unsigned int windowHeight
			);

			static void SetActiveRenderer(Renderer& renderer);
			static void ShutdownRenderer(Renderer& renderer);

			RenderCompositingNode& rootCompositingNode;

			void Update();

			RenderCompositingNode& AllocateCompositingNode(
				unsigned int tagGroup,
				unsigned int width,
				unsigned int height
			);
			void AttachCompositingNodeDependency(
				RenderCompositingNode& dependent,
				RenderCompositingNode& dependency
			);
			void DetachCompositingNodeDependency(
				RenderCompositingNode& dependent,
				RenderCompositingNode& dependency
			);
			void DeallocateCompositingNode(RenderCompositingNode& compositingNode);

			RenderCamera& AllocateCamera(
				const CameraKind& kind,
				float size,
				float aspectRatio,
				float nearClippingPlane,
				float farClippingPlane
			);
			void DeallocateCamera(RenderCamera& camera);

			void BindCompositingPair(
				RenderCamera& camera,
				RenderCompositingNode& compositingNode
			);

			void UnbindCompositingPair(
				RenderCamera& camera,
				RenderCompositingNode& compositingNode
			);

			RenderVertexShader& AllocateVertexShader(const string& shaderSource);
			void DeallocateVertexShader(const RenderVertexShader& vertexShader);

			RenderFragmentShader& AllocateFragmentShader(const string& shaderSource);
			void DeallocateFragmentShader(const RenderFragmentShader& fragmentShader);

			RenderShaderProgram& AllocateShaderProgram(
				const RenderVertexShader& vertexShader,
				const RenderFragmentShader& fragmentShader
			);
			void DeallocateShaderProgram(const RenderShaderProgram& shaderProgram);

			RenderMesh& AllocateMesh(
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
			void DeallocateMesh(RenderMesh& mesh);

			RenderSkeleton& AllocateSkeleton(
				const string& rootBone,
				const vector<RenderSkeleton::BoneConfig>& bones
			);
			void DeallocateSkeleton(RenderSkeleton& skeleton);

			RenderTexture& AllocateTexture(
				unsigned int width,
				unsigned int height,
				const unsigned char* data
			);
			void DeallocateTexture(RenderTexture& texture);

			RenderMaterial& AllocateMaterial();
			void DeallocateMaterial(RenderMaterial& material);

			RenderModel& AllocateModel();
			void SetModelMesh(RenderModel& model, RenderMesh* pMesh);
			void SetModelSkeleton(RenderModel& model, RenderSkeleton* pSkeleton);
			void SetModelMaterial(RenderModel& model, RenderMaterial* pMaterial);
			void DeallocateModel(RenderModel& model);

			void DeclareTagGroup(unsigned int tagGroup);
			void AddModelToTagGroup(RenderModel& model, unsigned int tagGroup);
			void RemoveModelFromTagGroup(RenderModel& model, unsigned int tagGroup);
			void RevokeTagGroup(unsigned int tagGroup);

		private:
			static const glm::mat4 MAT4_IDENTITY;

			SDL_Window& _window;
			SDL_GLContext& _glContext;

#ifdef __PRE_DEBUG__
			unordered_set<RenderCompositingNode*> _compositingNodes;

			unordered_set<const RenderCamera*> _cameras;
#endif

			unordered_map<RenderCompositingNode*, RenderCamera*> _compositingNodeCameraPairs;

#ifdef __PRE_DEBUG__
			// Note: this should be a 1:1 match to the above at all times, used for debugging
			unordered_map<const RenderCamera*, const RenderCompositingNode*> _cameraCompositingNodePairs;

			unordered_set<const RenderVertexShader*> _vertexShaders;
			unordered_set<const RenderFragmentShader*> _fragmentShaders;
			unordered_set<const RenderShaderProgram*> _shaderPrograms;

			unordered_set<const RenderMesh*> _meshes;
			unordered_set<const RenderSkeleton*> _skeletons;
			unordered_set<const RenderTexture*> _textures;
			unordered_set<const RenderMaterial*> _materials;

			unordered_set<const RenderModel*> _models;
#endif

			unordered_map<unsigned int, unordered_set<RenderModel*>> _tagGroups;

#ifdef __PRE_DEBUG__
			// used to track whether a model is still part of a tag group
			unordered_map<const RenderModel*, unordered_set<unsigned int>> _modelTagGroups;
#endif

			Renderer(SDL_Window& window, SDL_GLContext& glContext);
			~Renderer();

#ifdef __PRE_DEBUG__
			void UpdateRecurse(
				RenderCompositingNode& currentNode,
				unordered_set<RenderCompositingNode*>& rendered,
				unordered_set<RenderCompositingNode*>& visited);
#else
			void UpdateRecurse(
				RenderCompositingNode& currentNode,
				unordered_set<RenderCompositingNode*>& rendered
			);
#endif
		};
	} // namespace RenderingModule
} // namespace PRE