#include <core/subsystems/asset/preassetmanager.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb/stb_image.h>

#include <include/modules/asset.h>

#include <core/preapplicationcontext.h>

#include <core/subsystems/asset/preassetmanagerconfig.h>

#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/rendering/pretexture.h>
#include <core/subsystems/rendering/premesh.h>

namespace PRE
{
	namespace Core
	{
		using std::ifstream;
		using std::string;
		using std::stringstream;
		using std::vector;

		using PRE::AssetModule::AssetManager;

		PREAssetManager::Impl& PREAssetManager::Impl::MakeImpl(
			PREApplicationContext& applicationContext,
			AssetManager& assetManager
		)
		{
			return *(new Impl(applicationContext, assetManager));
		}

		PREAssetManager::Impl::Impl(
			PREApplicationContext& applicationContext,
			AssetManager& assetManager
		)
			:
			applicationContext(applicationContext),
			assetManager(assetManager) {}

		PREAssetManager::Impl::~Impl()
		{
			delete &assetManager;
		}

		PREShader& PREAssetManager::LoadShader(const string& vertexShaderPath, const string& fragmentShaderPath)
		{
			auto combinedPath = vertexShaderPath + "?" + fragmentShaderPath;
			auto vShader = _impl.assetManager.Get(combinedPath);
			if (vShader != nullptr)
			{
				return *static_cast<PREShader*>(vShader);
			}

			ifstream ifsVertex;
			ifsVertex.open(vertexShaderPath);

#ifdef __PRE_DEBUG__
			if (!ifsVertex.is_open())
			{
				throw "Failed to open Vertex Shader";
			}
#endif

			stringstream ssVertex;
			ssVertex << ifsVertex.rdbuf();
			ifsVertex.close();
			
			auto vertexSource = ssVertex.str();

			ifstream ifsFragment;
			ifsFragment.open(fragmentShaderPath);

#ifdef __PRE_DEBUG__
			if (!ifsFragment.is_open())
			{
				throw "Failed to open Vertex Shader";
			}
#endif	

			stringstream ssFragment;
			ssFragment << ifsFragment.rdbuf();
			ifsFragment.close();

			auto fragmentSource = ssFragment.str();

			auto& shader = _impl.applicationContext.rendering.CreateShader(
				vertexSource,
				fragmentSource
			);

			_impl.assetManager.Store(
				combinedPath,
				1,
				&shader,
				UnloadShader,
				&_impl.applicationContext.rendering
			);

			// load once more, this time guaranteed hit
			return LoadShader(vertexShaderPath, fragmentShaderPath);
		}

		PRETexture& PREAssetManager::LoadTexture(const string& texturePath)
		{
			auto vTexture = _impl.assetManager.Get(texturePath);
			if (vTexture != nullptr)
			{
				return *static_cast<PRETexture*>(vTexture);
			}

			stbi_set_flip_vertically_on_load(true);
			int width, height, nrComponents;
			auto pData = stbi_load(
				texturePath.c_str(),
				&width,
				&height,
				&nrComponents,
				4
			);
			
			auto& texture = _impl.applicationContext.rendering.CreateTexture();
			texture.SetData(width, height, pData);

			stbi_image_free(pData);

			_impl.assetManager.Store(
				texturePath,
				1,
				&texture,
				UnloadTexture,
				&_impl.applicationContext.rendering
			);

			// load once more, this time guaranteed hit
			return LoadTexture(texturePath);
		}

		PREMesh& PREAssetManager::LoadMesh(const string& meshPath)
		{
			auto vMesh = _impl.assetManager.Get(meshPath);
			if (vMesh != nullptr)
			{
				return *static_cast<PREMesh*>(vMesh);
			}

			auto aiScene = _impl.assimp.ReadFile(
				meshPath,
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_CalcTangentSpace |
				aiProcess_OptimizeMeshes
			);

#ifdef __PRE_DEBUG__
			if (aiScene == nullptr)
			{
				throw "Failed to load Mesh";
			}
#endif

			vector<glm::vec3> vertices;
			vector<glm::vec3> normals;
			vector<glm::vec2> uvs;
			vector<unsigned int> triangles;

			RecurseMesh(aiScene->mMeshes, aiScene->mRootNode, aiMatrix4x4(), vertices, normals, uvs, triangles);

			auto& mesh = _impl.applicationContext.rendering.CreateMesh();
			mesh.SetVertices(&vertices[0], (unsigned int)vertices.size());
			mesh.SetNormals(&normals[0], (unsigned int)normals.size());
			mesh.SetUvs(&uvs[0], (unsigned int)uvs.size());
			mesh.SetTriangles(&triangles[0], (unsigned int)triangles.size());

			_impl.assetManager.Store(
				meshPath,
				1,
				&mesh,
				UnloadMesh,
				&_impl.applicationContext.rendering
			);

			// load once more, this time guaranteed hit
			return LoadMesh(meshPath);
		}

		void PREAssetManager::FreeShader(const string& vertexShaderPath, const string& fragmentShaderPath)
		{
			auto combinedPath = vertexShaderPath + "?" + fragmentShaderPath;
			_impl.assetManager.Release(combinedPath);
		}

		void PREAssetManager::FreeTexture(const string& texturePath)
		{
			_impl.assetManager.Release(texturePath);
		}

		void PREAssetManager::FreeMesh(const string& meshPath)
		{
			_impl.assetManager.Release(meshPath);
		}

		void PREAssetManager::UnloadShader(void* vRendering, void* vShader)
		{
			auto pRendering = static_cast<PRERendering*>(vRendering);
			auto pShader = static_cast<PREShader*>(vShader);
			pRendering->DestroyShader(*pShader);
		}

		void PREAssetManager::UnloadTexture(void* vRendering, void* vTexture)
		{
			auto pRendering = static_cast<PRERendering*>(vRendering);
			auto pTexture = static_cast<PRETexture*>(vTexture);
			pRendering->DestroyTexture(*pTexture);
		}

		void PREAssetManager::UnloadMesh(void* vRendering, void* vMesh)
		{
			auto pRendering = static_cast<PRERendering*>(vRendering);
			auto pMesh = static_cast<PREMesh*>(vMesh);
			pRendering->DestroyMesh(*pMesh);
		}

		void PREAssetManager::RecurseMesh(
			aiMesh** pMeshes,
			aiNode* pCurrentNode,
			const aiMatrix4x4& localSpace,
			vector<glm::vec3>& vertices,
			vector<glm::vec3>& normals,
			vector<glm::vec2>& uvs,
			vector<unsigned int>& triangles
		)
		{
			auto currentTransform = pCurrentNode->mTransformation * localSpace;

			for (auto i = 0u; i < pCurrentNode->mNumMeshes; ++i)
			{
				auto vertexOffset = (unsigned int)vertices.size();
				auto pMesh = pMeshes[pCurrentNode->mMeshes[i]];
				for (auto j = 0u; j < pMesh->mNumVertices; ++j)
				{
					auto vertex = currentTransform * pMesh->mVertices[j];
					auto& normal = pMesh->mNormals[j];
					auto& uv = pMesh->mTextureCoords[0][j];

					vertices.push_back(glm::vec3(vertex.x, vertex.y, vertex.z));
					normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
					uvs.push_back(glm::vec2(uv.x, uv.y));
				}

				for (auto j = 0u; j < pMesh->mNumFaces; ++j)
				{
					auto& face = pMesh->mFaces[j];
					for (auto k = 0u; k < face.mNumIndices; ++k)
					{
						triangles.push_back(vertexOffset + face.mIndices[k]);
					}
				}
			}

			for (auto i = 0u; i < pCurrentNode->mNumChildren; ++i)
			{
				RecurseMesh(
					pMeshes,
					pCurrentNode->mChildren[i],
					currentTransform,
					vertices,
					normals,
					uvs,
					triangles
				);
			}
		}

		PREAssetManager& PREAssetManager::MakePREAssetManager(
			const PREAssetManagerConfig& assetManagerConfig,
			PREApplicationContext& applicationContext
		)
		{
			auto& assetManager = *(new AssetManager(0));
			return *(new PREAssetManager(
				assetManagerConfig.rootAssetPath,
				applicationContext,
				assetManager
			));
		}

		PREAssetManager::PREAssetManager(
			const string& rootAssetPath,
			PREApplicationContext& applicationContext,
			AssetManager& assetManager
		)
			:
			rootAssetPath(rootAssetPath),
			_impl(Impl::MakeImpl(applicationContext, assetManager)) {}

		PREAssetManager::~PREAssetManager()
		{
			delete &_impl;
		}

		void PREAssetManager::Initialize() {}

		void PREAssetManager::Update() {}

		void PREAssetManager::Shutdown() {}
	} // namespace Core
} // namespace PRE