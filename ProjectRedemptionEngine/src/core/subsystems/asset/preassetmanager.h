#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <include/modules/asset.h>

namespace PRE
{
	namespace Core
	{
		struct PREAssetManagerConfig;

		class PREApplicationContext;

		class PRERendering;

		class PREShader;
		class PREMesh;
		class PREBoneConfig;
		class PRESkeleton;
		class PRETexture;
		class PREAnimation;

		using std::pair;
		using std::string;
		using std::unordered_map;
		using std::vector;

		using PRE::AssetModule::AssetManager;

		class PREAssetManager
		{
			PREAssetManager& operator=(const PREAssetManager&) = delete;
			PREAssetManager(const PREAssetManager&) = delete;

			friend class PREApplicationContext;
			friend class PRERendering;

#pragma region Entries
			struct ShaderEntry
			{
				const string vertexShaderStringResource;
				const string fragmentShaderStringResource;

				ShaderEntry(
					const string& vertexShaderStringResource,
					const string& fragmentShaderStringResource
				);
			};

			struct TextureEntry
			{
				const string contentsImageResource;

				TextureEntry(const string& contentsImageResource);
			};

			struct MeshEntry
			{
				const string assimpResource;

				MeshEntry(const string& assimpResource);
			};

			struct SkeletonEntry {
				const string assimpResource;

				SkeletonEntry(const string& assimpResource);
			};

			struct AnimationEntry {
				const string assimpResource;

				AnimationEntry(const string& assimpResource);
			};
#pragma endregion

#pragma region Resources
			struct ResourceBase
			{
				virtual ~ResourceBase() = 0;
				virtual size_t GetSize() = 0;
			};

			struct StringResource : public ResourceBase
			{
				static StringResource* Load(const string& filepath);

				const string payload;

				~StringResource() override;

				size_t GetSize() override;

			private:
				StringResource(const string& payload);
			};

			struct ImageResource : public ResourceBase
			{
				static ImageResource* Load(const string& filepath);

				const unsigned int width;
				const unsigned int height;
				const unsigned char *const data;

				~ImageResource() override;

				size_t GetSize() override;

			private:
				ImageResource(
					unsigned int width,
					unsigned int height,
					const unsigned char* data
				);
			};

			struct AssimpResource : public ResourceBase
			{
				static AssimpResource* Load(
					Assimp::Importer& assimp,
					const string& filepath
				);

				~AssimpResource() override;

				const unsigned int nVertices;
				const glm::vec3* const vertices;
				const glm::vec3* const normals;
				const glm::vec3* const tangents;
				const glm::vec3* const biTangents;
				const glm::vec2* const uvs;
				const glm::ivec4* const vertexBoneInfluenceIndices;
				const glm::vec4* const vertexBoneInfluenceWeights;
				const unsigned int nTriangleElements;
				const unsigned int* triangleElements;

				const PREBoneConfig& rootBoneConfig;

				size_t GetSize() override;

			private:
				static void RecurseMesh(
					aiMesh** pMeshes,
					aiNode* pCurrentNode,
					vector<glm::vec3>& vertices,
					vector<glm::vec3>& normals,
					vector<glm::vec3>& tangents,
					vector<glm::vec3>& biTangents,
					vector<glm::vec2>& uvs,
					vector<glm::ivec4>& vertexBoneInfluenceIndices,
					vector<glm::vec4>& vertexBoneInfluenceWeights,
					vector<unsigned int>& triangleElements,
					unsigned int& boneCount,
					unordered_map<string, pair<aiBone*, unsigned int>>& boneMap,
					PREBoneConfig*& generatedBoneConfig,
					vector<PREBoneConfig*>& allBoneConfigs
				);

				static void GenerateBoneMap(
					aiMesh** pMeshes,
					aiNode* pCurrentNode,
					vector<glm::ivec4>& vertexBoneInfluenceIndices,
					vector<glm::vec4>& vertexBoneInfluenceWeights,
					unsigned int& vertexOffset,
					unordered_map<string, pair<aiBone*, unsigned int>>& boneMap
				);

				const unsigned int _nBoneConfigs;
				const PREBoneConfig* const * _allBoneConfigs;

				AssimpResource(
					unsigned int nVertices,
					const glm::vec3* vertices,
					const glm::vec3* normals,
					const glm::vec3* tangents,
					const glm::vec3* biTangents,
					const glm::vec2* uvs,
					const glm::ivec4* vertexBoneInfluenceIndices,
					const glm::vec4* vertexBoneInfluenceWeights,
					unsigned int nTriangleElements,
					const unsigned int* triangleElements,
					const PREBoneConfig& rootBoneConfig,
					unsigned int nBoneConfigs,
					const PREBoneConfig* const * allBoneConfigs
				);
			};
#pragma endregion

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;
				
				friend class PREAssetManager;

			private:
				static Impl& MakeImpl(
					PREApplicationContext& applicationContext,
					AssetManager& assetManager
				);

				Assimp::Importer assimp;

				unordered_map<const PREShader*, ShaderEntry*> shaders;
				unordered_map<const PRETexture*, TextureEntry*> textures;
				unordered_map<const PREMesh*, MeshEntry*> meshes;
				unordered_map<const PRESkeleton*, SkeletonEntry*> skeletons;
				unordered_map<const PREAnimation*, AnimationEntry*> animations;

				PREApplicationContext& applicationContext;
				AssetManager& assetManager;

				Impl(
					PREApplicationContext& applicationContext,
					AssetManager& assetManager
				);
				~Impl();
			};

		public:
			const string rootAssetPath;

			PREShader& LoadShader(const string& vertexShaderPath, const string& fragmentShaderPath);
			PRETexture& LoadTexture(const string& texturePath);
			PREMesh& LoadMesh(const string& meshPath);
			PRESkeleton& LoadSkeleton(const string& skeletonPath);
			PREAnimation& LoadAnimation(const string& animationPath);

		private:
			static void UnloadResourceData(void* vNil, void* vResource);

			static PREAssetManager& MakePREAssetManager(
				const PREAssetManagerConfig& assetManagerConfig,
				PREApplicationContext& applicationContext
			);

			// Will attempt to free resources allocated by the
			// object if it was allocated by the asset manager
			// no-op otherwise. Should only be called by relevant
			// subsystems.

			void TryFreeShader(const PREShader& shader);
			void TryFreeTexture(const PRETexture& texture);
			void TryFreeMesh(const PREMesh& mesh);
			void TryFreeSkeleton(const PRESkeleton& skeleton);
			void TryFreeAnimation(const PREAnimation& animation);

			Impl& _impl;

			PREAssetManager(
				const string& rootAssetPath,
				PREApplicationContext& applicationContext,
				AssetManager& assetManager
			);
			~PREAssetManager();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE