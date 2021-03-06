#include <core/subsystems/asset/preassetmanager.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
#include <core/subsystems/rendering/preskeleton.h>
#include <core/subsystems/rendering/preboneconfig.h>
#include <core/subsystems/rendering/preanimation.h>
#include <core/subsystems/rendering/preanimationconfig.h>
#include <core/subsystems/rendering/preanimationchannelconfig.h>
#include <core/subsystems/rendering/preskybox.h>

#ifdef __PRE_DEBUG__
#include <assert.h>
#endif

namespace PRE
{
	namespace Core
	{
		using std::ifstream;
		using std::pair;
		using std::string;
		using std::stringstream;
		using std::unordered_map;
		using std::vector;

		using PRE::AssetModule::AssetManager;

#pragma region Entries
		PREAssetManager::ShaderEntry::ShaderEntry(
			const string& vertexShaderStringResource,
			const string& fragmentShaderStringResource
		)
			:
			vertexShaderStringResource(vertexShaderStringResource),
			fragmentShaderStringResource(fragmentShaderStringResource) {}

		PREAssetManager::TextureEntry::TextureEntry(
			const string& contentsImageResource
		)
			:
			contentsImageResource(contentsImageResource) {}

		PREAssetManager::MeshEntry::MeshEntry(const string& assimpResource)
			:
			assimpResource(assimpResource) {}

		PREAssetManager::SkeletonEntry::SkeletonEntry(const string& assimpResource)
			:
			assimpResource(assimpResource) {}

		PREAssetManager::AnimationEntry::AnimationEntry(const string& assimpResource)
			:
			assimpResource(assimpResource) {}

		PREAssetManager::SkyBoxEntry::SkyBoxEntry(
			const string& rightFace,
			const string& leftFace,
			const string& topFace,
			const string& bottomFace,
			const string& frontFace,
			const string& backFace
		)
			:
			rightFace(rightFace),
			leftFace(leftFace),
			topFace(topFace),
			bottomFace(bottomFace),
			frontFace(frontFace),
			backFace(backFace) {}
#pragma endregion

		PREAssetManager::ResourceBase::~ResourceBase() {}

#pragma region StringResource
		PREAssetManager::StringResource* PREAssetManager::StringResource::Load(
			const string& filepath
		)
		{
			ifstream ifs;
			ifs.open(filepath);


			if (!ifs.is_open())
			{
				throw std::exception((string("Failed to open ") + filepath).c_str());
			}

			stringstream ss;
			ss << ifs.rdbuf();
			ifs.close();
			return new StringResource(std::move(ss.str()));
		}

		PREAssetManager::StringResource::~StringResource() {}

		size_t PREAssetManager::StringResource::GetSize()
		{
			return payload.length() * sizeof(char);
		}

		PREAssetManager::StringResource::StringResource(
			string&& payload
		)
			: payload(std::move(payload)) {}
#pragma endregion

#pragma region ImageResource
		PREAssetManager::ImageResource* PREAssetManager::ImageResource::Load(
			const string& filepath,
			bool flipVerticallyOnLoad // TODO: this is such a hack...
		)
		{
			stbi_set_flip_vertically_on_load(flipVerticallyOnLoad);

			int width, height, nrComponents;
			auto pDataStbi = stbi_load(
				filepath.c_str(),
				&width,
				&height,
				&nrComponents,
				4
			);

			auto imageSize = width * height * 4;
			auto pData = new unsigned char[imageSize];
			std::memcpy(pData, pDataStbi, imageSize);
			stbi_image_free(pDataStbi);

			return new ImageResource(width, height, pData);
		}

		PREAssetManager::ImageResource::~ImageResource()
		{
			delete[] data;
		}

		size_t PREAssetManager::ImageResource::GetSize()
		{
			return
				sizeof(unsigned int) +
				sizeof(unsigned int) +
				((size_t)width) * ((size_t)height) * 4 * sizeof(unsigned char);
		}

		PREAssetManager::ImageResource::ImageResource(
			unsigned int width,
			unsigned int height,
			const unsigned char* data
		)
			:
			width(width),
			height(height),
			data(data) {}
#pragma endregion

#pragma region AssimpResource
		PREAssetManager::AssimpResource* PREAssetManager::AssimpResource::Load(
			Assimp::Importer& assimp,
			const string& filepath
		)
		{
			auto aiScene = assimp.ReadFile(
				filepath,
				aiProcess_Triangulate |
				aiProcess_LimitBoneWeights |
				aiProcess_GenSmoothNormals |
				aiProcess_CalcTangentSpace |
				aiProcess_OptimizeMeshes
			);

			if (aiScene == nullptr)
			{
				throw std::exception((string("Failed to load resource ") + filepath).c_str());
			}

			vector<glm::ivec4> vVertexBoneInfluenceIndices;
			vector<glm::vec4> vVertexBoneInfluenceWeights;
			unsigned int vertexOffset = 0;
			unordered_map<string, pair<aiBone*, unsigned int>> boneMap;

			AssimpResource::GenerateBoneMap(
				aiScene->mMeshes,
				aiScene->mRootNode,
				vVertexBoneInfluenceIndices,
				vVertexBoneInfluenceWeights,
				vertexOffset,
				boneMap
			);

			vector<glm::vec3> vVertices;
			vector<glm::vec3> vNormals;
			vector<glm::vec3> vTangents;
			vector<glm::vec3> vBiTangents;
			vector<glm::vec2> vUvs;
			vector<unsigned int> vTriangleElements;
			unsigned int boneCount = 0;
			PREBoneConfig* pRootBoneConfig;

			vector<PREBoneConfig*> vAllBoneConfigs;

			AssimpResource::RecurseMesh(
				aiScene->mMeshes,
				aiScene->mRootNode,
				vVertices,
				vNormals,
				vTangents,
				vBiTangents,
				vUvs,
				vVertexBoneInfluenceIndices,
				vVertexBoneInfluenceWeights,
				vTriangleElements,
				boneCount,
				boneMap,
				pRootBoneConfig,
				vAllBoneConfigs
			);

			auto nVertices = (unsigned int)vVertices.size();
			auto nTriangleElements = (unsigned int)vTriangleElements.size();

			auto vertices = new glm::vec3[nVertices];
			std::memcpy(vertices, &vVertices[0], nVertices * sizeof(glm::vec3));
			
			auto normals = new glm::vec3[nVertices];
			std::memcpy(normals, &vNormals[0], nVertices * sizeof(glm::vec3));
			
			auto tangents = new glm::vec3[nVertices];
			std::memcpy(tangents, &vTangents[0], nVertices * sizeof(glm::vec3));
			
			auto biTangents = new glm::vec3[nVertices];
			std::memcpy(biTangents, &vBiTangents[0], nVertices * sizeof(glm::vec3));
			
			auto uvs = new glm::vec2[nVertices];
			std::memcpy(uvs, &vUvs[0], nVertices * sizeof(glm::vec2));
			
			auto vertexBoneInfluenceIndices = new glm::ivec4[nVertices];
			std::memcpy(
				vertexBoneInfluenceIndices,
				&vVertexBoneInfluenceIndices[0],
				nVertices * sizeof(glm::ivec4)
			);

			auto vertexBoneInfluenceWeights = new glm::vec4[nVertices];
			std::memcpy(
				vertexBoneInfluenceWeights,
				&vVertexBoneInfluenceWeights[0],
				nVertices * sizeof(glm::vec4)
			);

			auto triangleElements = new unsigned int[nTriangleElements];
			std::memcpy(triangleElements, &vTriangleElements[0], nTriangleElements * sizeof(unsigned int));

			auto nBoneConfigs = (unsigned int)vAllBoneConfigs.size();
			auto allBoneConfigs = new PREBoneConfig* [nBoneConfigs];
			std::memcpy(allBoneConfigs, &vAllBoneConfigs[0], nBoneConfigs * sizeof(PREBoneConfig*));

			unordered_map<string, const PREAnimationConfig*> animationConfigs;
			GenerateAnimations(aiScene, animationConfigs);

			return new AssimpResource(
				nVertices,
				vertices,
				normals,
				tangents,
				biTangents,
				uvs,
				vertexBoneInfluenceIndices,
				vertexBoneInfluenceWeights,
				nTriangleElements,
				triangleElements,
				*pRootBoneConfig,
				nBoneConfigs,
				allBoneConfigs,
				std::move(animationConfigs)
			);
		}

		PREAssetManager::AssimpResource::~AssimpResource()
		{
			delete[] vertices;
			delete[] normals;
			delete[] tangents;
			delete[] biTangents;
			delete[] uvs;
			delete[] vertexBoneInfluenceIndices;
			delete[] vertexBoneInfluenceWeights;
			delete[] triangleElements;
			for (auto i = 0u; i < _nBoneConfigs; ++i)
			{
				delete _allBoneConfigs[i];
			}
			delete[] _allBoneConfigs;
			for (auto it = animationConfigs.begin(); it != animationConfigs.end(); ++it)
			{
				delete it->second;
			}
		}

		size_t PREAssetManager::AssimpResource::GetSize()
		{
			return
				((size_t)nVertices) *
				(
					4 * sizeof(glm::vec3) +
					sizeof(glm::vec2) +
					sizeof(glm::ivec4) +
					sizeof(glm::vec4)
				); // + sizeof preboneconfigs... whatever for now...
		}

		void PREAssetManager::AssimpResource::RecurseMesh(
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
			PREBoneConfig* & generatedBoneConfig,
			vector<PREBoneConfig*>& allBoneConfigs
		)
		{
			string currentNodeName(pCurrentNode->mName.C_Str());

			auto itPBone = boneMap.find(currentNodeName);
			auto bindPos = aiMatrix4x4();
			if (itPBone != boneMap.end())
			{
				auto pBone = itPBone->second.first;
				bindPos = pBone->mOffsetMatrix;

				auto vertexOffset = itPBone->second.second;
				for (auto k = 0u; k < pBone->mNumWeights; ++k)
				{
					auto& weight = pBone->mWeights[k];
					auto vertexIndex = vertexOffset + weight.mVertexId;
					auto& indices = vertexBoneInfluenceIndices[vertexIndex];
					auto& weights = vertexBoneInfluenceWeights[vertexIndex];

#ifdef __PRE_DEBUG__
					assert(indices[3] == -1);
#endif

					for (auto i = 0; i < 4; ++i)
					{
						if (indices[i] == -1)
						{
							indices[i] = boneCount;
							weights[i] = weight.mWeight;
							break;
						}
					}
				}
			}

			generatedBoneConfig = new PREBoneConfig(
				boneCount++,
				currentNodeName,
				glm::transpose(glm::make_mat4(&bindPos.a1)),
				glm::transpose(glm::make_mat4(&pCurrentNode->mTransformation.a1))
			);

			allBoneConfigs.push_back(generatedBoneConfig);

			for (auto i = 0u; i < pCurrentNode->mNumMeshes; ++i)
			{
				auto vertexOffset = (unsigned int)vertices.size();
				auto pMesh = pMeshes[pCurrentNode->mMeshes[i]];

				std::for_each(
					pMesh->mVertices,
					pMesh->mVertices + pMesh->mNumVertices,
					[&vertices](const auto& aiVertex) {
						vertices.push_back(glm::vec3(aiVertex.x, aiVertex.y, aiVertex.z));
					}
				);

				if (pMesh->HasNormals())
				{
					std::for_each(
						pMesh->mNormals,
						pMesh->mNormals + pMesh->mNumVertices,
						[&normals](const auto& aiNormal) {
							normals.push_back(glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z));
						}
					);
				}
				else
				{
					std::for_each(
						pMesh->mVertices,
						pMesh->mVertices + pMesh->mNumVertices,
						[&normals](const auto& _) { normals.push_back(glm::vec3()); }
					);
				}

				if (pMesh->HasTangentsAndBitangents())
				{
					std::for_each(
						pMesh->mTangents,
						pMesh->mTangents + pMesh->mNumVertices,
						[&tangents](const auto& aiTangent) {
							tangents.push_back(glm::vec3(aiTangent.x, aiTangent.y, aiTangent.z));
						}
					);
					std::for_each(
						pMesh->mBitangents,
						pMesh->mBitangents + pMesh->mNumVertices,
						[&biTangents](const auto& aiBiTangent) {
							biTangents.push_back(glm::vec3(aiBiTangent.x, aiBiTangent.y, aiBiTangent.z));
						}
					);
				}
				else
				{
					std::for_each(
						pMesh->mVertices,
						pMesh->mVertices + pMesh->mNumVertices,
						[&tangents, &biTangents](const auto& _) {
							tangents.push_back(glm::vec3());
							biTangents.push_back(glm::vec3());
						}
					);
				}

				if (pMesh->HasTextureCoords(0))
				{
					std::for_each(
						pMesh->mTextureCoords[0],
						pMesh->mTextureCoords[0] + pMesh->mNumVertices,
						[&uvs](const auto& aiUv) { uvs.push_back(glm::vec2(aiUv.x, aiUv.y)); }
					);
				}
				else
				{
					std::for_each(
						pMesh->mVertices,
						pMesh->mVertices + pMesh->mNumVertices,
						[&uvs](const auto& _) { uvs.push_back(glm::vec2()); }
					);
				}

				if (pMesh->HasFaces())
				{
					std::for_each(
						pMesh->mFaces,
						pMesh->mFaces + pMesh->mNumFaces,
						[&triangleElements, &vertexOffset](const auto& aiFace)
						{
#if __PRE_DEBUG__
							assert(aiFace.mNumIndices == 3);
#endif

							for (auto i = 0u; i < aiFace.mNumIndices; ++i)
							{
								triangleElements.push_back(
									vertexOffset + aiFace.mIndices[i]
								);
							}
						}
					);
				}
			}

			for (auto i = 0u; i < pCurrentNode->mNumChildren; ++i)
			{
				PREBoneConfig* generatedChildConfig = nullptr;
				RecurseMesh(
					pMeshes,
					pCurrentNode->mChildren[i],
					vertices,
					normals,
					tangents,
					biTangents,
					uvs,
					vertexBoneInfluenceIndices,
					vertexBoneInfluenceWeights,
					triangleElements,
					boneCount,
					boneMap,
					generatedChildConfig,
					allBoneConfigs
				);
				generatedBoneConfig->AddChild(*generatedChildConfig);
			}
		}

		void PREAssetManager::AssimpResource::GenerateBoneMap(
			aiMesh** pMeshes,
			aiNode* pCurrentNode,
			vector<glm::ivec4>& vertexBoneInfluenceIndices,
			vector<glm::vec4>& vertexBoneInfluenceWeights,
			unsigned int& vertexOffset,
			unordered_map<string, pair<aiBone*, unsigned int>>& boneMap
		)
		{
			for (auto i = 0u; i < pCurrentNode->mNumMeshes; ++i)
			{
				auto pMesh = pMeshes[pCurrentNode->mMeshes[i]];

				std::for_each(
					pMesh->mVertices,
					pMesh->mVertices + pMesh->mNumVertices,
					[&vertexBoneInfluenceIndices, &vertexBoneInfluenceWeights](const auto _)
					{
						vertexBoneInfluenceIndices.push_back(glm::ivec4(-1));
						vertexBoneInfluenceWeights.push_back(glm::vec4());
					}
				);

				std::for_each(
					pMesh->mBones,
					pMesh->mBones + pMesh->mNumBones,
					[&boneMap, &vertexOffset](const auto& pBone)
					{
						string boneName(pBone->mName.C_Str());

#ifdef __PRE_DEBUG__
						assert(boneMap.find(boneName) == boneMap.end());
#endif

						boneMap.insert(
							std::make_pair(
								boneName,
								std::make_pair(
									pBone,
									vertexOffset
								)
							)
						);
					}
				);

				vertexOffset += pMesh->mNumVertices;
			}

			for (auto i = 0u; i < pCurrentNode->mNumChildren; ++i)
			{
				GenerateBoneMap(
					pMeshes,
					pCurrentNode->mChildren[i],
					vertexBoneInfluenceIndices,
					vertexBoneInfluenceWeights,
					vertexOffset,
					boneMap
				);
			}
		}

		void PREAssetManager::AssimpResource::GenerateAnimations(
			const aiScene* pScene,
			unordered_map<string, const PREAnimationConfig*>& animationConfigs
		)
		{
			std::for_each(
				pScene->mAnimations,
				pScene->mAnimations + pScene->mNumAnimations,
				[&animationConfigs](const auto& pAnimation)
				{
					auto pAnimConfig = new PREAnimationConfig(
						(float)pAnimation->mTicksPerSecond,
						(float)pAnimation->mDuration
					);

					std::for_each(
						pAnimation->mChannels,
						pAnimation->mChannels + pAnimation->mNumChannels,
						[&pAnimConfig](const auto& pAnimChannel)
						{
							auto& channelConfig = pAnimConfig->AddAnimationChannelConfig(
								string(pAnimChannel->mNodeName.C_Str())
							);
							std::for_each(
								pAnimChannel->mPositionKeys,
								pAnimChannel->mPositionKeys + pAnimChannel->mNumPositionKeys,
								[&channelConfig](const auto& key)
								{
									channelConfig.AddPositionKeyFrame(
										(float)key.mTime,
										glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z)
									);
								}
							);
							std::for_each(
								pAnimChannel->mRotationKeys,
								pAnimChannel->mRotationKeys + pAnimChannel->mNumRotationKeys,
								[&channelConfig](const auto& key)
								{
									channelConfig.AddRotationKeyFrame(
										(float)key.mTime,
										glm::fquat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z)
									);
								}
							);
							std::for_each(
								pAnimChannel->mScalingKeys,
								pAnimChannel->mScalingKeys + pAnimChannel->mNumScalingKeys,
								[&channelConfig](const auto& key)
								{
									channelConfig.AddScaleKeyFrame(
										(float)key.mTime,
										glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z)
									);
								}
							);
						}
					);
					animationConfigs[pAnimation->mName.C_Str()] = pAnimConfig;
				}
			);
		}

		PREAssetManager::AssimpResource::AssimpResource(
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
			const PREBoneConfig* const * allBoneConfigs,
			unordered_map<string, const PREAnimationConfig*>&& animationConfigs
		)
			:
			nVertices(nVertices),
			vertices(vertices),
			normals(normals),
			tangents(tangents),
			biTangents(biTangents),
			uvs(uvs),
			vertexBoneInfluenceIndices(vertexBoneInfluenceIndices),
			vertexBoneInfluenceWeights(vertexBoneInfluenceWeights),
			nTriangleElements(nTriangleElements),
			triangleElements(triangleElements),
			rootBoneConfig(rootBoneConfig),
			_nBoneConfigs(nBoneConfigs),
			_allBoneConfigs(allBoneConfigs),
			animationConfigs(std::move(animationConfigs)) {}
#pragma endregion

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
#ifdef __PRE_DEBUG__
			assert(shaders.size() == 0);
			assert(textures.size() == 0);
			assert(meshes.size() == 0);
			assert(skeletons.size() == 0);
			assert(animations.size() == 0);
			assert(skyBoxes.size() == 0);
#endif
			delete &assetManager;
		}

		PREShader& PREAssetManager::LoadShader(
			const string& vertexShaderPath,
			const string& fragmentShaderPath
		)
		{
			auto vShader = _impl.assetManager.Get(vertexShaderPath);
			if (vShader == nullptr)
			{
				auto pVertexShaderResource = StringResource::Load(
					vertexShaderPath
				);
				_impl.assetManager.Store(
					vertexShaderPath,
					pVertexShaderResource->GetSize(),
					pVertexShaderResource,
					UnloadResourceData,
					nullptr
				);

				return LoadShader(vertexShaderPath, fragmentShaderPath);
			}

			auto fShader = _impl.assetManager.Get(fragmentShaderPath);
			if (fShader == nullptr)
			{
				_impl.assetManager.Release(vertexShaderPath);

				auto pFragmentShaderResource = StringResource::Load(
					fragmentShaderPath
				);
				_impl.assetManager.Store(
					fragmentShaderPath,
					pFragmentShaderResource->GetSize(),
					pFragmentShaderResource,
					UnloadResourceData,
					nullptr
				);

				return LoadShader(vertexShaderPath, fragmentShaderPath);
			}

			auto vertexSource = static_cast<StringResource*>(vShader)->payload.c_str();
			auto fragmentSource = static_cast<StringResource*>(fShader)->payload.c_str();

			auto& shader = _impl.applicationContext.rendering.CreateShader(
				vertexSource,
				fragmentSource
			);
			
			_impl.shaders[&shader] = new ShaderEntry(
				vertexShaderPath,
				fragmentShaderPath
			);

			return shader;
		}

		PRETexture& PREAssetManager::LoadTexture(const string& texturePath)
		{
			auto vImageResource = _impl.assetManager.Get(texturePath);
			if (vImageResource == nullptr)
			{
				auto pImageResource = ImageResource::Load(texturePath, true);
				_impl.assetManager.Store(
					texturePath,
					pImageResource->GetSize(),
					pImageResource,
					UnloadResourceData,
					nullptr
				);

				return LoadTexture(texturePath);
			}

			auto& imageResource = *static_cast<ImageResource*>(vImageResource);

			auto& texture = _impl.applicationContext.rendering.CreateTexture(
				imageResource.width,
				imageResource.height,
				imageResource.data
			);

			_impl.textures[&texture] = new TextureEntry(texturePath);

			return texture;
		}

		PREMesh& PREAssetManager::LoadMesh(const string& meshPath)
		{
			auto vAssimpResource = _impl.assetManager.Get(meshPath);
			if (vAssimpResource == nullptr)
			{
				auto pAssimpResource = AssimpResource::Load(
					_impl.assimp,
					meshPath
				);

				_impl.assetManager.Store(
					meshPath,
					pAssimpResource->GetSize(),
					pAssimpResource,
					UnloadResourceData,
					nullptr
				);

				return LoadMesh(meshPath);
			}

			auto& assimpResource = *static_cast<AssimpResource*>(vAssimpResource);

			auto& mesh = _impl.applicationContext.rendering.CreateMesh(
				assimpResource.nVertices,
				assimpResource.vertices,
				assimpResource.normals,
				assimpResource.tangents,
				assimpResource.biTangents,
				assimpResource.uvs,
				assimpResource.vertexBoneInfluenceIndices,
				assimpResource.vertexBoneInfluenceWeights,
				assimpResource.nTriangleElements,
				assimpResource.triangleElements
			);

			_impl.meshes[&mesh] = new MeshEntry(meshPath);

			return mesh;
		}

		PRESkeleton& PREAssetManager::LoadSkeleton(const string& skeletonPath)
		{
			auto vAssimpResource = _impl.assetManager.Get(skeletonPath);
			if (vAssimpResource == nullptr)
			{
				auto pAssimpResource = AssimpResource::Load(
					_impl.assimp,
					skeletonPath
				);

				_impl.assetManager.Store(
					skeletonPath,
					pAssimpResource->GetSize(),
					pAssimpResource,
					UnloadResourceData,
					nullptr
				);

				return LoadSkeleton(skeletonPath);
			}

			auto& assimpResource = *static_cast<AssimpResource*>(vAssimpResource);

			auto& skeleton = _impl.applicationContext.rendering.CreateSkeleton(
				assimpResource.rootBoneConfig
			);

			_impl.skeletons[&skeleton] = new SkeletonEntry(skeletonPath);

			return skeleton;
		}

		PREAnimation& PREAssetManager::LoadAnimation(
			const string& animationPath,
			const string& animationName
		)
		{
			auto vAssimpResource = _impl.assetManager.Get(animationPath);
			if (vAssimpResource == nullptr)
			{
				auto pAssimpResource = AssimpResource::Load(
					_impl.assimp,
					animationPath
				);

				_impl.assetManager.Store(
					animationPath,
					pAssimpResource->GetSize(),
					pAssimpResource,
					UnloadResourceData,
					nullptr
				);

				return LoadAnimation(animationPath, animationName);
			}

			auto& assimpResource = *static_cast<AssimpResource*>(vAssimpResource);

#ifdef __PRE_DEBUG__
			assert(assimpResource.animationConfigs.find(animationName) != assimpResource.animationConfigs.end());
#endif

			auto& animation = _impl.applicationContext.rendering.CreateAnimation(
				*assimpResource.animationConfigs.find(animationName)->second
			);

			_impl.animations[&animation] = new AnimationEntry(animationPath);

			return animation;
		}

		PRESkyBox& PREAssetManager::LoadSkyBox(
			const string& rightPath,
			const string& leftPath,
			const string& topPath,
			const string& bottomPath,
			const string& frontPath,
			const string& backPath
		)
		{
			const string* const paths[] { &rightPath, &leftPath, &topPath, &bottomPath, &frontPath, &backPath };
			unordered_map<const string*, void*> imageResources;

			for (auto i = 0; i < 6; ++i)
			{
				const auto& path = *paths[i];
				auto vImageResource = _impl.assetManager.Get(path);
				if (vImageResource == nullptr)
				{
					for (auto it = imageResources.begin(); it != imageResources.end(); ++it)
					{
						_impl.assetManager.Release(*it->first);
					}

					auto pImageResource = ImageResource::Load(path, false);
					_impl.assetManager.Store(
						path,
						pImageResource->GetSize(),
						pImageResource,
						UnloadResourceData,
						nullptr
					);

					return LoadSkyBox(rightPath, leftPath, topPath, bottomPath, frontPath, backPath);
				}
				imageResources[&path] = vImageResource;
			}

			auto& right = *static_cast<ImageResource*>(imageResources.find(&rightPath)->second);
			auto& left = *static_cast<ImageResource*>(imageResources.find(&leftPath)->second);
			auto& top = *static_cast<ImageResource*>(imageResources.find(&topPath)->second);
			auto& bottom = *static_cast<ImageResource*>(imageResources.find(&bottomPath)->second);
			auto& front = *static_cast<ImageResource*>(imageResources.find(&frontPath)->second);
			auto& back = *static_cast<ImageResource*>(imageResources.find(&backPath)->second);

			auto& skybox = _impl.applicationContext.rendering.CreateSkyBox(
				right.width, right.height, right.data,
				left.width, left.height, left.data,
				top.width, top.height, top.data,
				bottom.width, bottom.height, bottom.data,
				front.width, front.height, front.data,
				back.width, back.height, back.data
			);

			_impl.skyBoxes[&skybox] = new SkyBoxEntry(rightPath, leftPath, topPath, bottomPath, frontPath, backPath);

			return skybox;
		}

		void PREAssetManager::UnloadResourceData(void* vNil, void* vResource)
		{
			delete static_cast<ResourceBase*>(vResource);
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

		void PREAssetManager::TryFreeShader(const PREShader& shader)
		{
			auto it = _impl.shaders.find(&shader);
			if (it == _impl.shaders.end())
			{
				return;
			}
			auto pShaderEntry = it->second;

			_impl.assetManager.Release(pShaderEntry->vertexShaderStringResource);
			_impl.assetManager.Release(pShaderEntry->fragmentShaderStringResource);

			delete pShaderEntry;

			_impl.shaders.erase(it);
		}

		void PREAssetManager::TryFreeTexture(const PRETexture& texture)
		{
			auto it = _impl.textures.find(&texture);
			if (it == _impl.textures.end())
			{
				return;
			}
			auto pTextureEntry = it->second;

			_impl.assetManager.Release(pTextureEntry->contentsImageResource);

			delete pTextureEntry;

			_impl.textures.erase(it);
		}

		void PREAssetManager::TryFreeMesh(const PREMesh& mesh)
		{
			auto it = _impl.meshes.find(&mesh);
			if (it == _impl.meshes.end())
			{
				return;
			}
			auto pMeshEntry = it->second;

			_impl.assetManager.Release(pMeshEntry->assimpResource);

			delete pMeshEntry;

			_impl.meshes.erase(it);
		}

		void PREAssetManager::TryFreeSkeleton(const PRESkeleton& skeleton)
		{
			auto it = _impl.skeletons.find(&skeleton);
			if (it == _impl.skeletons.end())
			{
				return;
			}
			auto pSkeletonEntry = it->second;

			_impl.assetManager.Release(pSkeletonEntry->assimpResource);

			delete pSkeletonEntry;

			_impl.skeletons.erase(it);
		}

		void PREAssetManager::TryFreeAnimation(const PREAnimation& animation)
		{
			auto it = _impl.animations.find(&animation);
			if (it == _impl.animations.end())
			{
				return;
			}
			auto pAnimationEntry = it->second;

			_impl.assetManager.Release(pAnimationEntry->assimpResource);

			delete pAnimationEntry;

			_impl.animations.erase(it);
		}

		void PREAssetManager::TryFreeSkyBox(const PRESkyBox& skyBox)
		{
			auto it = _impl.skyBoxes.find(&skyBox);
			if (it == _impl.skyBoxes.end())
			{
				return;
			}
			auto pSkyboxEntry = it->second;

			_impl.assetManager.Release(pSkyboxEntry->rightFace);
			_impl.assetManager.Release(pSkyboxEntry->leftFace);
			_impl.assetManager.Release(pSkyboxEntry->topFace);
			_impl.assetManager.Release(pSkyboxEntry->bottomFace);
			_impl.assetManager.Release(pSkyboxEntry->frontFace);
			_impl.assetManager.Release(pSkyboxEntry->backFace);

			delete pSkyboxEntry;

			_impl.skyBoxes.erase(it);
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