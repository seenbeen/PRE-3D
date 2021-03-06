#pragma once
#include <string>
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

		class PREShader;
		class PREMesh;
		class PRETexture;

		using std::string;
		using std::vector;

		using PRE::AssetModule::AssetManager;

		class PREAssetManager
		{
			PREAssetManager& operator=(const PREAssetManager&) = delete;
			PREAssetManager(const PREAssetManager&) = delete;

			friend class PREApplicationContext;

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

			void FreeShader(const string& vertexShaderPath, const string& fragmentShaderPath);
			void FreeTexture(const string& texturePath);
			void FreeMesh(const string& meshPath);

		private:
			static void UnloadShader(void* vRendering, void* vShader);
			static void UnloadTexture(void* vRendering, void* vTexture);
			static void UnloadMesh(void* vRendering, void* vMesh);

			static void RecurseMesh(
				aiMesh** pMeshes,
				aiNode* currentNode,
				const aiMatrix4x4& localSpace,
				vector<glm::vec3>& vertices,
				vector<glm::vec3>& normals,
				vector<glm::vec2>& uvs,
				vector<unsigned int>& triangles
			);

			static PREAssetManager& MakePREAssetManager(
				const PREAssetManagerConfig& assetManagerConfig,
				PREApplicationContext& applicationContext
			);

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