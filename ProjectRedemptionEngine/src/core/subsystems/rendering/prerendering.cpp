#include <core/subsystems/rendering/prerendering.h>

#include <algorithm>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <glm/glm.hpp>

#include <include/modules/rendering.h>
#include <include/modules/animation.h>

#include <core/preapplicationcontext.h>

#include <core/components/precameracomponent.h>
#include <core/components/premodelrenderercomponent.h>
#include <core/components/pretransformcomponent.h>
#include <core/components/preambientlightcomponent.h>
#include <core/components/prepointlightcomponent.h>
#include <core/components/prespotlightcomponent.h>
#include <core/components/predirectionallightcomponent.h>

#include <core/subsystems/asset/preassetmanager.h>

#include <core/subsystems/rendering/prerenderingconfig.h>

#include <core/subsystems/rendering/prerendertexture.h>
#include <core/subsystems/rendering/preshader.h>
#include <core/subsystems/rendering/premesh.h>
#include <core/subsystems/rendering/preskeleton.h>
#include <core/subsystems/rendering/preboneconfig.h>
#include <core/subsystems/rendering/pretexture.h>
#include <core/subsystems/rendering/prematerial.h>

#include <core/subsystems/rendering/preanimator.h>
#include <core/subsystems/rendering/preanimation.h>

#include <core/subsystems/rendering/preskybox.h>

#include <core/subsystems/rendering/prelightrenderpassdata.h>
#include <core/subsystems/rendering/prelightrenderpasscontext.h>
#include <core/subsystems/rendering/preshadowrenderpassdata.h>
#include <core/subsystems/rendering/preshadowrenderpasscontext.h>

#ifdef __PRE_DEBUG__
#include <assert.h>
#include <iostream>
#endif

namespace PRE
{
	namespace Core
	{
		using std::set;
		using std::string;
		using std::vector;
		using std::unordered_map;
		using std::unordered_set;

		using PRE::RenderingModule::Renderer;
		using PRE::RenderingModule::RenderCompositingNode;
		using PRE::RenderingModule::RenderCamera;
		using PRE::RenderingModule::RenderShaderProgram;
		using PRE::RenderingModule::RenderMesh;
		using PRE::RenderingModule::RenderMaterial;
		using PRE::RenderingModule::RenderModel;

		using PRE::AnimationModule::Animation;

#pragma region Screen Display
		static const string SCREEN_SAMPLER_KEY = "screenSampler";
		static const int SCREEN_SAMPLER_UNIT = 0;
		static const string SCREEN_VERTEX_SHADER_SOURCE =
			"#version 330 core\n"
			"layout (location = 0) in vec3 iPos;\n"
			"layout (location = 4) in vec2 iUv;\n"
			"\n"
			"out vec2 TexCoord;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    TexCoord = iUv;\n"
			"    gl_Position = vec4(iPos, 1.0f);\n"
			"}\n";

		static const string SCREEN_FRAGMENT_SHADER_SOURCE =
			"#version 330 core\n"
			"in vec2 TexCoord;\n"
			"\n"
			"out vec4 FragColor;\n"
			"\n"
			"uniform sampler2D screenSampler;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    FragColor = texture(screenSampler, TexCoord);\n"
			"}\n";

		static const unsigned int SCREEN_VERTEX_COUNT = 4u;
		static const glm::vec3 SCREEN_VERTICES[]
		{
			glm::vec3(-1, -1, 0),
			glm::vec3(1, -1, 0),
			glm::vec3(1, 1, 0),
			glm::vec3(-1, 1, 0)
		};
		static const glm::vec2 SCREEN_UVS[]
		{
			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1)
		};
		static const unsigned int SCREEN_TRIANGLE_ELEMENT_COUNT = 6u;
		static const unsigned int SCREEN_TRIANGLE_ELEMENTS[]
		{
			0u, 1u, 2u, 0u, 2u, 3u
		};
#pragma endregion

		PRERendering::Impl& PRERendering::Impl::MakeImpl(
			PREApplicationContext& applicationContext,
			Renderer& renderer,
			PRERendering& rendering
		)
		{
			auto& screenVertexShader = renderer.AllocateVertexShader(SCREEN_VERTEX_SHADER_SOURCE);
			auto& screenFragmentShader = renderer.AllocateFragmentShader(SCREEN_FRAGMENT_SHADER_SOURCE);
			auto& screenShaderProgram = renderer.AllocateShaderProgram(screenVertexShader, screenFragmentShader);
			renderer.DeallocateVertexShader(screenVertexShader);
			renderer.DeallocateFragmentShader(screenFragmentShader);
			screenShaderProgram.SetInt(SCREEN_SAMPLER_KEY, SCREEN_SAMPLER_UNIT);
			auto& screenMesh = renderer.AllocateMesh(
				SCREEN_VERTEX_COUNT,
				SCREEN_VERTICES,
				nullptr,
				nullptr,
				nullptr,
				SCREEN_UVS,
				nullptr,
				nullptr,
				SCREEN_TRIANGLE_ELEMENT_COUNT,
				SCREEN_TRIANGLE_ELEMENTS
			);
			auto& screenMaterial = renderer.AllocateMaterial();
			auto& screenModel = renderer.AllocateModel();
			screenMaterial.SetShaderProgram(&screenShaderProgram);
			auto& screenCamera = renderer.AllocateCamera(
				Renderer::CameraKind::ORTHOGRAPHIC,
				1,
				renderer.width / (float)renderer.height,
				-1,
				1
			);
			renderer.SetModelMesh(screenModel, &screenMesh);
			renderer.SetModelMaterial(screenModel, &screenMaterial);

			return *(
				new Impl(
					applicationContext,
					renderer,
					rendering,
					screenShaderProgram,
					screenMesh,
					screenMaterial,
					screenModel,
					screenCamera,
					renderer.AllocateDepthCompositingTarget(SHADOW_MAP_SIZE, false),
					renderer.AllocateCamera(
						// these can be whatever; they'll be assigned prior to shadow mapping
						Renderer::CameraKind::ORTHOGRAPHIC,
						1, 1, 0, 1
					)
				)
			);
		}

		void PRERendering::Impl::ScreenOnRender(
			RenderCompositingNode::RenderComposition& composition,
			void* vRenderingImpl
		)
		{
			auto& renderingImpl = *static_cast<Impl*>(vRenderingImpl);
			auto& screenTexture = renderingImpl.rendering._screenRenderTexture;
			renderingImpl.screenMaterial.SetTextureBinding(
				screenTexture._accumulatorBufferIsA ?
						screenTexture._pBufferB :
						screenTexture._pBufferA
				,
				SCREEN_SAMPLER_UNIT
			);

			composition.SetCompositingTarget(nullptr);
			composition.AddModel(renderingImpl.screenModel);
			composition.SetCamera(&renderingImpl.screenCamera);
			composition.Clear();
		}

		PRERendering::Impl::Impl(
			PREApplicationContext& applicationContext,
			Renderer& renderer,
			PRERendering& rendering,
			RenderShaderProgram& screenShaderProgram,
			RenderMesh& screenMesh,
			RenderMaterial& screenMaterial,
			RenderModel& screenModel,
			RenderCamera& screenCamera,
			RenderCompositingTarget& shadowMap2D,
			RenderCamera& lightPOVCamera
		)
			:
			applicationContext(applicationContext),
			renderer(renderer),
			rendering(rendering),
			screenCompositingNode(renderer.AllocateCompositingNode(ScreenOnRender, this)),
			screenShaderProgram(screenShaderProgram),
			screenMesh(screenMesh),
			screenMaterial(screenMaterial),
			screenModel(screenModel),
			screenCamera(screenCamera),
			rootRenderPassData(new PRELightRenderPassData(screenCompositingNode)),
			shadowMap2D(shadowMap2D),
			// shadowMap3D(renderer.AllocateCompositingTarget(SHADOW_MAP_SIZE, true),
			lightPOVCamera(lightPOVCamera)
		{
			modelTagMap.insert(
				std::make_pair(
					0,
					std::move(unordered_set<PREModelRendererComponent*>())
				)
			);
		}

		PRERendering::Impl::~Impl()
		{
			renderer.DeallocateCamera(lightPOVCamera);
			// renderer.DeallocateCompositingTarget(shadowMap3D);
			renderer.DeallocateCompositingTarget(shadowMap2D);
			delete rootRenderPassData;
			renderer.DeallocateCamera(screenCamera);
			renderer.DeallocateModel(screenModel);
			renderer.DeallocateMaterial(screenMaterial);
			renderer.DeallocateMesh(screenMesh);
			renderer.DeallocateShaderProgram(screenShaderProgram);
			renderer.DeallocateCompositingNode(screenCompositingNode);
			Renderer::ShutdownRenderer(renderer);
		}

		void PRERendering::Impl::UnlinkCompositingChains()
		{
			auto previousNode = rootRenderPassData->pNode;
			for (auto it = compositingChains.rbegin(); it != compositingChains.rend(); ++it)
			{
				auto itLightRenderPassData = it->second.begin();
				if (itLightRenderPassData != it->second.end())
				{
					renderer.DetachCompositingNodeDependency(
						*previousNode,
						*(*itLightRenderPassData)->pNode
					);
					previousNode = (*(--it->second.end()))->pNode;
				}
			}
		}

		void PRERendering::Impl::RelinkCompositingChains()
		{
			auto previousNode = rootRenderPassData->pNode;
			for (auto it = compositingChains.rbegin(); it != compositingChains.rend(); ++it)
			{
				auto itLightRenderPassData = it->second.begin();
				if (itLightRenderPassData != it->second.end())
				{
					renderer.AttachCompositingNodeDependency(
						*previousNode,
						*(*itLightRenderPassData)->pNode
					);
					previousNode = (*(--it->second.end()))->pNode;
				}
			}
		}

		void PRERendering::Impl::LinkLightToRenderTarget(
			PRERenderTexture& renderPass,
			PRELightRenderPassData& lightData,
			void* pLightComponent,
			list<PRELightRenderPassData*>& compositingChain,
			list<PRELightRenderPassData*>::iterator& itLightFront,
			PREShadowRenderPassData* pShadowData
		)
		{
			auto pRenderPass = &renderPass;
			auto pLightData = &lightData;

			// link shadow pass
			if (pShadowData != nullptr && pShadowData->pLastLightData == nullptr)
			{
				renderer.AttachCompositingNodeDependency(
					*pLightData->pNode,
					*pShadowData->pNode
				);
				pShadowData->pLastLightData = pLightData;
			}

			PRELightRenderPassData* pPreviousLightData = nullptr;
			if (itLightFront != compositingChain.begin())
			{
				auto itLightPrevious(itLightFront);
				pPreviousLightData = *(--itLightPrevious);
			}

			PRELightRenderPassData* pNextLightData = nullptr;
			if (itLightFront != compositingChain.end())
			{
				pNextLightData = *itLightFront;
			}

			// detach if both previous and next exist
			if (pPreviousLightData != nullptr && pNextLightData != nullptr)
			{
				renderer.DetachCompositingNodeDependency(
					*pPreviousLightData->pNode,
					*pNextLightData->pNode
				);
			}

			// splice in new light if previous exists
			if (pPreviousLightData != nullptr)
			{
				renderer.AttachCompositingNodeDependency(
					*pPreviousLightData->pNode,
					*pLightData->pNode
				);
			}

			// re-attach next light if exists
			if (pNextLightData != nullptr)
			{
				renderer.AttachCompositingNodeDependency(
					*pLightData->pNode,
					*pNextLightData->pNode
				);
			}

			// insert light render pass data
			itLightFront = compositingChain.insert(itLightFront, pLightData);

			// link point light

#ifdef __PRE_DEBUG__
			assert(renderPass._lightMap.find(pLightComponent) == renderPass._lightMap.end());
#endif

			renderPass._lightMap[pLightComponent] = itLightFront;
		}

		void PRERendering::Impl::UnlinkLightFromRenderTarget(
			PRERenderTexture& renderPass,
			PRELightRenderPassData& lightData,
			void* pLightComponent,
			list<PRELightRenderPassData*>& compositingChain,
			list<PRELightRenderPassData*>::iterator& itLightFront,
			PREShadowRenderPassData* pShadowData
		)
		{
			auto pRenderPass = &renderPass;
			auto pRemovedLightData = &lightData;

#ifdef __PRE_DEBUG__
			assert(pRenderPass->_lightMap.find(pLightComponent) != pRenderPass->_lightMap.end());
#endif

			auto itRemovedLightData(renderPass._lightMap.find(pLightComponent)->second);

			// re-link shadow pass if necessary
			if (pShadowData != nullptr && pShadowData->pLastLightData == pRemovedLightData)
			{
				pShadowData->pLastLightData = nullptr;
				renderer.DetachCompositingNodeDependency(
					*pRemovedLightData->pNode,
					*pShadowData->pNode
				);
				if (itRemovedLightData != itLightFront)
				{
					auto itSecondLastLightData(itRemovedLightData);
					auto pSecondLastLightData = *--itSecondLastLightData;
					renderer.AttachCompositingNodeDependency(
						*pSecondLastLightData->pNode,
						*pShadowData->pNode
					);
					pShadowData->pLastLightData = pSecondLastLightData;
				}
			}

			PRELightRenderPassData* pPreviousLightData = nullptr;
			if (itRemovedLightData != compositingChain.begin())
			{
				auto itPreviousLightData(itRemovedLightData);
				pPreviousLightData = *(--itPreviousLightData);
			}

			PRELightRenderPassData* pNextLightData = nullptr;
			auto itNextLightData(itRemovedLightData);
			if (++itNextLightData != compositingChain.end())
			{
				pNextLightData = *itNextLightData;
			}

			// unlink previous light if exists
			if (pPreviousLightData != nullptr)
			{
				renderer.DetachCompositingNodeDependency(
					*pPreviousLightData->pNode,
					*pRemovedLightData->pNode
				);
			}

			// unlink next light if exists
			if (pNextLightData != nullptr)
			{
				renderer.DetachCompositingNodeDependency(
					*pRemovedLightData->pNode,
					*pNextLightData->pNode
				);
			}

			// re-splice if both previous and next exist
			if (pPreviousLightData != nullptr && pNextLightData != nullptr)
			{
				renderer.AttachCompositingNodeDependency(
					*pPreviousLightData->pNode,
					*pNextLightData->pNode
				);
			}

			// erase light render pass data
			if (itRemovedLightData == itLightFront)
			{
				++itLightFront;
			}
			compositingChain.erase(itRemovedLightData);

			pRenderPass->_lightMap.erase(pLightComponent);
		}

		const int PRERendering::DEFAULT_LAYER = 0;

		PRERenderTexture& PRERendering::GetScreenRenderTexture()
		{
			return _screenRenderTexture;
		}

		PRERenderTexture& PRERendering::CreateRenderTexture(int layer, unsigned int width, unsigned int height)
		{
			auto pRenderTexture = new PRERenderTexture(
				layer,
				glm::vec2(width, height),
				_impl.renderer.AllocateCompositingTarget(width, height),
				_impl.renderer.AllocateCompositingTarget(width, height)
			);
			auto itCompositingChain = _impl.compositingChains.find(layer);
			if (itCompositingChain == _impl.compositingChains.end())
			{
				auto itBegin = _impl.compositingChains.insert(
					std::make_pair(
						layer,
						std::move(list<PRELightRenderPassData*>())
					)
				).first->second.begin();
				for (auto itLight = _impl.ambientLights.begin(); itLight != _impl.ambientLights.end(); ++itLight)
				{
					(*itLight)->_fronts[layer] = itBegin;
				}
				for (auto itLight = _impl.pointLights.begin(); itLight != _impl.pointLights.end(); ++itLight)
				{
					(*itLight)->_fronts[layer] = itBegin;
				}
				for (auto itLight = _impl.spotLights.begin(); itLight != _impl.spotLights.end(); ++itLight)
				{
					auto& spotLightComponent = **itLight;
					spotLightComponent._fronts[layer] = itBegin;
					auto pShadowContext = new PREShadowRenderPassContext(
						_impl.lightPOVCamera,
						_impl.modelTagMap,
						_impl.shadowMap2D,
						spotLightComponent
					);
					auto pNewShadowData = new PREShadowRenderPassData(
						_impl.renderer.AllocateCompositingNode(
							ShadowPassOnRender,
							pShadowContext
						),
						*pShadowContext
					);

					spotLightComponent._shadowPassData[layer] = pNewShadowData;
				}
				for (auto itLight = _impl.directionalLights.begin(); itLight != _impl.directionalLights.end(); ++itLight)
				{
					auto& directionalLightComponent = **itLight;
					directionalLightComponent._fronts[layer] = itBegin;
					auto pShadowContext = new PREShadowRenderPassContext(
						_impl.lightPOVCamera,
						_impl.modelTagMap,
						_impl.shadowMap2D,
						directionalLightComponent
					);
					auto pNewShadowData = new PREShadowRenderPassData(
						_impl.renderer.AllocateCompositingNode(
							ShadowPassOnRender,
							pShadowContext
						),
						*pShadowContext
					);

					directionalLightComponent._shadowPassData[layer] = pNewShadowData;
				}
			}

			LinkRenderTextureToLights(*pRenderTexture);
			return *pRenderTexture;
		}

		void PRERendering::DestroyRenderTexture(PRERenderTexture& renderTexture)
		{
			auto pRenderTexture = &renderTexture;
			if (renderTexture._pAssociatedCameraComponent != nullptr)
			{
				UnlinkCameraComponentFromRenderTexture(
					*renderTexture._pAssociatedCameraComponent,
					renderTexture
				);
			}

			UnlinkRenderTextureFromLights(*pRenderTexture);

			auto itCompositingChain = _impl.compositingChains.find(renderTexture._layer);
			if (itCompositingChain->second.size() == 0)
			{
				_impl.compositingChains.erase(itCompositingChain);
				for (auto itLight = _impl.ambientLights.begin(); itLight != _impl.ambientLights.end(); ++itLight)
				{
					(*itLight)->_fronts.erase(renderTexture._layer);
				}
				for (auto itLight = _impl.pointLights.begin(); itLight != _impl.pointLights.end(); ++itLight)
				{
					(*itLight)->_fronts.erase(renderTexture._layer);
				}
				for (auto itLight = _impl.spotLights.begin(); itLight != _impl.spotLights.end(); ++itLight)
				{
					auto& spotLightComponent = **itLight;
					auto pRemovedShadowData = spotLightComponent._shadowPassData.at(renderTexture._layer);
					_impl.renderer.DeallocateCompositingNode(*pRemovedShadowData->pNode);
					delete pRemovedShadowData->pRenderPassContext;
					delete pRemovedShadowData;
					spotLightComponent._shadowPassData.erase(renderTexture._layer);
					spotLightComponent._fronts.erase(renderTexture._layer);
				}
				for (auto itLight = _impl.directionalLights.begin(); itLight != _impl.directionalLights.end(); ++itLight)
				{
					auto& directionalLightComponent = **itLight;
					auto pRemovedShadowData = directionalLightComponent._shadowPassData.at(renderTexture._layer);
					_impl.renderer.DeallocateCompositingNode(*pRemovedShadowData->pNode);
					delete pRemovedShadowData->pRenderPassContext;
					delete pRemovedShadowData;
					directionalLightComponent._shadowPassData.erase(renderTexture._layer);
					directionalLightComponent._fronts.erase(renderTexture._layer);
				}
			}
			_impl.renderer.DeallocateCompositingTarget(*renderTexture._pBufferA);
			_impl.renderer.DeallocateCompositingTarget(*renderTexture._pBufferB);
			delete &renderTexture;
		}

		PREShader& PRERendering::CreateShader(const string& vertex, const string& fragment)
		{
			auto& vertexShader = _impl.renderer.AllocateVertexShader(vertex);
			auto& fragmentShader = _impl.renderer.AllocateFragmentShader(fragment);
			auto pShader = new PREShader(
				_impl.renderer.AllocateShaderProgram(vertexShader, fragmentShader)
			);
			_impl.renderer.DeallocateFragmentShader(fragmentShader);
			_impl.renderer.DeallocateVertexShader(vertexShader);
			pShader->_shaderProgram.SetDepthFunction(RenderShaderProgram::DepthFunction::LESS_THAN_OR_EQUAL);
			return *pShader;
		}

		void PRERendering::DestroyShader(PREShader& shader)
		{
			_impl.renderer.DeallocateShaderProgram(shader._shaderProgram);
			_impl.applicationContext.assetManager.TryFreeShader(shader);
			delete& shader;
		}

		PRETexture& PRERendering::CreateTexture(
			unsigned int width,
			unsigned int height,
			const unsigned char* data
		)
		{
			return *(
				new PRETexture(
					_impl.renderer.AllocateTexture(
						width,
						height,
						data
					)
				)
			);
		}

		void PRERendering::DestroyTexture(PRETexture& texture)
		{
			_impl.renderer.DeallocateTexture(texture._texture);
			_impl.applicationContext.assetManager.TryFreeTexture(texture);
			delete& texture;
		}

		PREMaterial& PRERendering::CreateMaterial()
		{
			return *(new PREMaterial(_impl.renderer.AllocateMaterial()));
		}

		void PRERendering::DestroyMaterial(PREMaterial& material)
		{
			_impl.renderer.DeallocateMaterial(material._material);
			delete &material;
		}

		PREMesh& PRERendering::CreateMesh(
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
		)
		{
			return *(new PREMesh(_impl.renderer.AllocateMesh(
				nVertices,
				vertices,
				normals,
				tangents,
				biTangents,
				uvs,
				vertexBoneIds,
				vertexBoneWeights,
				nTriangleElements,
				triangleElements
			)));
		}

		void PRERendering::DestroyMesh(PREMesh& mesh)
		{
			_impl.renderer.DeallocateMesh(mesh._mesh);
			_impl.applicationContext.assetManager.TryFreeMesh(mesh);
			delete& mesh;
		}

		PRESkeleton& PRERendering::CreateSkeleton(
			const PREBoneConfig& rootBoneConfig
		)
		{
			return *(
				new PRESkeleton(
					_impl.renderer.AllocateSkeleton(
						rootBoneConfig._boneConfig
					)
				)
			);
		}

		void PRERendering::DestroySkeleton(PRESkeleton& skeleton)
		{
			_impl.renderer.DeallocateSkeleton(skeleton._skeleton);
			_impl.applicationContext.assetManager.TryFreeSkeleton(skeleton);
			delete& skeleton;
		}

		PREAnimation& PRERendering::CreateAnimation(
			const PREAnimationConfig& animationConfig
		)
		{
			return *(new PREAnimation(animationConfig));
		}

		void PRERendering::DestroyAnimation(PREAnimation& animation)
		{
			_impl.applicationContext.assetManager.TryFreeAnimation(animation);
			delete& animation;
		}

		PREAnimator& PRERendering::CreateAnimator(
			const PREAnimatorConfig& animatorConfig
		)
		{
			return *(new PREAnimator(animatorConfig));
		}

		void PRERendering::DestroyAnimator(PREAnimator& animator)
		{
			delete &animator;
		}

#pragma region SkyBox
		static const string SKYBOX_SAMPLER_KEY = "skyboxCubeSampler";
		static const int SKYBOX_SAMPLER_UNIT = 0;
		static const string SKYBOX_VERTEX_SHADER_SOURCE =
			"#version 330 core\n"
			"layout (location = 0) in vec3 iPos;\n"
			"\n"
			"uniform mat4 PRE_MODEL_MATRIX;\n"
			"uniform mat4 PRE_VIEW_MATRIX;\n"
			"uniform mat4 PRE_PROJECTION_MATRIX;\n"
			"\n"
			"out vec3 TexCoord;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    vec4 pos = PRE_PROJECTION_MATRIX * mat4(mat3(PRE_VIEW_MATRIX * PRE_MODEL_MATRIX)) * vec4(iPos, 1.0);\n"
			"    TexCoord = iPos;\n"
			"    gl_Position = pos.xyww;\n"
			"}\n";

		static const string SKYBOX_FRAGMENT_SHADER_SOURCE =
			"#version 330 core\n"
			"in vec3 TexCoord;\n"
			"\n"
			"out vec4 FragColor;\n"
			"\n"
			"uniform samplerCube skyboxCubeSampler;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    FragColor = texture(skyboxCubeSampler, TexCoord);\n"
			"}\n";

		static const unsigned int SKYBOX_VERTEX_COUNT = 8u;
		static const glm::vec3 SKYBOX_VERTICES[]
		{
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1, 1, 1),
			glm::vec3(-1, 1, 1),
			glm::vec3(-1, -1, -1),
			glm::vec3(1, -1, -1),
			glm::vec3(1, 1, -1),
			glm::vec3(-1, 1, -1),
		};
		static const unsigned int SKYBOX_TRIANGLE_ELEMENT_COUNT = 36u;
		static const unsigned int SKYBOX_TRIANGLE_ELEMENTS[]
		{
			5u, 2u, 6u, 5u, 1u, 2u, // right
			0u, 7u, 3u, 0u, 4u, 7u, // left
			7u, 2u, 3u, 7u, 6u, 2u, // top
			5u, 0u, 1u, 5u, 4u, 0u, // bottom
			4u, 6u, 7u, 4u, 5u, 6u, // front
			1u, 3u, 2u, 1u, 0u, 3u // back
		};
#pragma endregion

		PRESkyBox& PRERendering::CreateSkyBox(
			unsigned int rightWidth,
			unsigned int rightHeight,
			const unsigned char* rightData,
			unsigned int leftWidth,
			unsigned int leftHeight,
			const unsigned char* leftData,
			unsigned int topWidth,
			unsigned int topHeight,
			const unsigned char* topData,
			unsigned int bottomWidth,
			unsigned int bottomHeight,
			const unsigned char* bottomData,
			unsigned int frontWidth,
			unsigned int frontHeight,
			const unsigned char* frontData,
			unsigned int backWidth,
			unsigned int backHeight,
			const unsigned char* backData
		)
		{
			auto& vShader = _impl.renderer.AllocateVertexShader(SKYBOX_VERTEX_SHADER_SOURCE);
			auto& fShader = _impl.renderer.AllocateFragmentShader(SKYBOX_FRAGMENT_SHADER_SOURCE);

			auto& shaderProgram = _impl.renderer.AllocateShaderProgram(vShader, fShader);
			_impl.renderer.DeallocateVertexShader(vShader);
			_impl.renderer.DeallocateFragmentShader(fShader);

			shaderProgram.SetInt(SKYBOX_SAMPLER_KEY, SKYBOX_SAMPLER_UNIT);
			shaderProgram.SetDepthWrite(false);
			shaderProgram.SetDepthFunction(RenderShaderProgram::DepthFunction::LESS_THAN_OR_EQUAL);

			auto& mesh = _impl.renderer.AllocateMesh(
				SKYBOX_VERTEX_COUNT,
				SKYBOX_VERTICES,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				SKYBOX_TRIANGLE_ELEMENT_COUNT,
				SKYBOX_TRIANGLE_ELEMENTS
			);

			auto& texture = _impl.renderer.AllocateTexture(
				rightWidth, rightHeight, rightData,
				leftWidth, leftHeight, leftData,
				topWidth, topHeight, topData,
				bottomWidth, bottomHeight, bottomData,
				frontWidth, frontHeight, frontData,
				backWidth, backHeight, backData
			);
			auto& material = _impl.renderer.AllocateMaterial();
			material.SetShaderProgram(&shaderProgram);
			material.SetTextureBinding(&texture, SKYBOX_SAMPLER_UNIT);
			auto& model = _impl.renderer.AllocateModel();
			_impl.renderer.SetModelMesh(model, &mesh);
			_impl.renderer.SetModelMaterial(model, &material);
			
			return *(new PRESkyBox(shaderProgram, mesh, texture, material, model));
		}

		void PRERendering::DestroySkyBox(PRESkyBox& skyBox)
		{
			// UnlinkCameraComponentFromSkyBox mutates _associatedCameraComponents
			// so we'll need to make vector copy before unlinking.
			vector<PRECameraComponent*> cameraComponents(
				skyBox._associatedCameraComponents.begin(),
				skyBox._associatedCameraComponents.end()
			);
			for (auto it = cameraComponents.begin(); it != cameraComponents.end(); ++it)
			{
				UnlinkCameraComponentFromSkyBox(**it, skyBox);
			}

			_impl.renderer.DeallocateShaderProgram(skyBox._shaderProgram);
			_impl.renderer.DeallocateMesh(skyBox._mesh);
			_impl.renderer.DeallocateTexture(skyBox._texture);
			_impl.renderer.DeallocateMaterial(skyBox._material);
			_impl.renderer.DeallocateModel(skyBox._model);
			_impl.applicationContext.assetManager.TryFreeSkyBox(skyBox);

			delete &skyBox;
		}

		void PRERendering::LightPassOnRender(
			RenderCompositingNode::RenderComposition& composition,
			void* vLightPassContext
		)
		{
			auto& lightPassContext = *static_cast<PRELightRenderPassContext*>(vLightPassContext);
			if (lightPassContext._renderTexture._pAssociatedCameraComponent != nullptr)
			{
				auto pCameraComponent = lightPassContext._renderTexture._pAssociatedCameraComponent;
				pCameraComponent->AllocateIfNotAllocated();

				int tag;
				if (lightPassContext._pAmbientLightComponent != nullptr)
				{
					tag = lightPassContext._pAmbientLightComponent->_tag;
				}
				else if (lightPassContext._pPointLightComponent != nullptr)
				{
					tag = lightPassContext._pPointLightComponent->_tag;
				}
				else if (lightPassContext._pSpotLightComponent != nullptr)
				{
					tag = lightPassContext._pSpotLightComponent->_tag;
				}
				else
				{
#ifdef __PRE_DEBUG__
					assert(lightPassContext._pDirectionalLightComponent != nullptr);
#endif
					tag = lightPassContext._pDirectionalLightComponent->_tag;
				}

				if (tag != pCameraComponent->_tag)
				{
					return;
				}

				unordered_set<PREModelRendererComponent*>* pModels = nullptr;
				auto itModels = lightPassContext._modelTagMap.find(tag);
				if (itModels == lightPassContext._modelTagMap.end())
				{
					return;
				}

				composition.SetCamera(pCameraComponent->_pCamera);

				auto pAccumulatorWrite = lightPassContext._renderTexture._accumulatorBufferIsA ?
					lightPassContext._renderTexture._pBufferA :
					lightPassContext._renderTexture._pBufferB;

				auto pAccumulatorRead = lightPassContext._renderTexture._accumulatorBufferIsA ?
					lightPassContext._renderTexture._pBufferB :
					lightPassContext._renderTexture._pBufferA;

				lightPassContext._renderTexture._accumulatorBufferIsA = !lightPassContext._renderTexture._accumulatorBufferIsA;

				auto isFirstPass = lightPassContext._renderTexture._passCounter == 0;
				auto isSecondPass = lightPassContext._renderTexture._passCounter == 1;

				++lightPassContext._renderTexture._passCounter;

				pModels = &itModels->second;

				// TODO: Spatial query optimizing to only render visible models
				for (auto it = pModels->begin(); it != pModels->end(); ++it)
				{
					auto& modelRendererComponent = **it;

					if (
						modelRendererComponent._pModel == nullptr ||
						modelRendererComponent._pMaterial == nullptr ||
						modelRendererComponent._pMaterial->_pShader == nullptr
					)
					{
						continue;
					}

					modelRendererComponent._pMaterial->_material.SetTextureBinding(
						pAccumulatorRead,
						PREShader::LIGHT_ACCUMULATOR_BINDING
					);
					modelRendererComponent._pMaterial->BindRenderTextureAccumulatorBindings();

					// shader program may have been temporarily set by shadow
					// casting, so reset any of these temp changes from last pass
					// modelRendererComponent._pMaterial->ResetShaderProgram();

					auto pShader = modelRendererComponent._pMaterial->_pShader;
					pShader->SetInt(
						PREShader::IS_FIRST_LIGHT_PASS,
						isFirstPass ? 1 : 0
					);
					pShader->SetVec2(
						PREShader::LIGHT_ACCUMULATOR_SAMPLER_SIZE,
						lightPassContext._renderTexture._size
					);
					pShader->SetInt(
						PREShader::LIGHT_ACCUMULATOR_SAMPLER,
						PREShader::LIGHT_ACCUMULATOR_BINDING
					);
					pShader->SetInt(PREShader::AMBIENT_LIGHT_FLAG, 0);
					pShader->SetInt(PREShader::POINT_LIGHT_FLAG, 0);
					pShader->SetInt(PREShader::SPOT_LIGHT_FLAG, 0);
					pShader->SetInt(PREShader::DIRECTIONAL_LIGHT_FLAG, 0);
					pShader->SetVec3(
						PREShader::VIEW_POSITION,
						pCameraComponent->_pTransformComponent->GetPosition()
					);

					if (lightPassContext._pAmbientLightComponent != nullptr)
					{
						auto pAmbientLightComponent = lightPassContext._pAmbientLightComponent;
						pAmbientLightComponent->AllocateIfNotAllocated();
						pShader->SetInt(PREShader::AMBIENT_LIGHT_FLAG, 1);
						pShader->SetVec3(
							PREShader::LIGHT_POSITION,
							pAmbientLightComponent->_pTransform->GetPosition()
						);
						pShader->SetVec3(
							PREShader::LIGHT_COLOR,
							pAmbientLightComponent->_color
						);
						pShader->SetFloat(
							PREShader::LIGHT_ATTENUATION_LINEAR,
							pAmbientLightComponent->_attenuationLinear
						);
						pShader->SetFloat(
							PREShader::LIGHT_ATTENUATION_QUADRATIC,
							pAmbientLightComponent->_attenuationQuadratic
						);
					}
					else if (lightPassContext._pPointLightComponent != nullptr)
					{
						auto pPointLightComponent = lightPassContext._pPointLightComponent;
						pPointLightComponent->AllocateIfNotAllocated();
						pShader->SetInt(PREShader::POINT_LIGHT_FLAG, 1);
						pShader->SetVec3(
							PREShader::LIGHT_POSITION,
							pPointLightComponent->_pTransform->GetPosition()
						);
						pShader->SetVec3(
							PREShader::LIGHT_COLOR,
							pPointLightComponent->_color
						);
						pShader->SetFloat(
							PREShader::LIGHT_ATTENUATION_LINEAR,
							pPointLightComponent->_attenuationLinear
						);
						pShader->SetFloat(
							PREShader::LIGHT_ATTENUATION_QUADRATIC,
							pPointLightComponent->_attenuationQuadratic
						);
					}
					else if (lightPassContext._pSpotLightComponent != nullptr)
					{
						auto pSpotLightComponent = lightPassContext._pSpotLightComponent;

#ifdef __PRE_DEBUG__
						assert(pSpotLightComponent->_pTransform != nullptr);
#endif

						pShader->SetInt(PREShader::SPOT_LIGHT_FLAG, 1);

						// shadow map
						modelRendererComponent._pMaterial->_material.SetTextureBinding(
							lightPassContext._pShadowMap,
							PREShader::SHADOW_MAP_BINDING
						);
						pShader->SetInt(
							PREShader::SHADOW_MAP_SAMPLER,
							PREShader::SHADOW_MAP_BINDING
						);
						pShader->SetMat4(
							PREShader::SHADOW_MAP_VIEW_MATRIX,
							pSpotLightComponent->_pTransform->GetInverseMatrix()
						);
						pShader->SetMat4(
							PREShader::SHADOW_MAP_PROJECTION_MATRIX,
							glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f)
						);

						pShader->SetVec3(
							PREShader::LIGHT_POSITION,
							pSpotLightComponent->_pTransform->GetPosition()
						);
						pShader->SetVec3(
							PREShader::LIGHT_COLOR,
							pSpotLightComponent->_color
						);
						pShader->SetVec3(
							PREShader::LIGHT_DIRECTION,
							pSpotLightComponent->_pTransform->GetForward()
						);
						pShader->SetFloat(
							PREShader::LIGHT_ATTENUATION_LINEAR,
							pSpotLightComponent->_attenuationLinear
						);
						pShader->SetFloat(
							PREShader::LIGHT_ATTENUATION_QUADRATIC,
							pSpotLightComponent->_attenuationQuadratic
						);
						pShader->SetFloat(
							PREShader::LIGHT_INNER_RADIUS,
							pSpotLightComponent->_innerRadius
						);
						pShader->SetFloat(
							PREShader::LIGHT_OUTER_RADIUS,
							pSpotLightComponent->_outerRadius
						);
					}
					else if (lightPassContext._pDirectionalLightComponent != nullptr)
					{
						auto pDirectionalLightComponent = lightPassContext._pDirectionalLightComponent;

#ifdef __PRE_DEBUG__
						assert(pDirectionalLightComponent->_pTransform != nullptr);
#endif

						pShader->SetInt(PREShader::DIRECTIONAL_LIGHT_FLAG, 1);

						// shadow map
						modelRendererComponent._pMaterial->_material.SetTextureBinding(
							lightPassContext._pShadowMap,
							PREShader::SHADOW_MAP_BINDING
						);
						pShader->SetInt(
							PREShader::SHADOW_MAP_SAMPLER,
							PREShader::SHADOW_MAP_BINDING
						);
						pShader->SetMat4(
							PREShader::SHADOW_MAP_VIEW_MATRIX,
							pDirectionalLightComponent->_pTransform->GetInverseMatrix()
						);
						pShader->SetMat4(
							PREShader::SHADOW_MAP_PROJECTION_MATRIX,
							glm::ortho(-6.0f, 6.0f, -6.0f, 6.0f, -10.0f, 10.0f)
						);

						pShader->SetVec3(
							PREShader::LIGHT_COLOR,
							pDirectionalLightComponent->_color
						);
						pShader->SetVec3(
							PREShader::LIGHT_DIRECTION,
							pDirectionalLightComponent->_pTransform->GetForward()
						);
					}
					composition.AddModel(*modelRendererComponent._pModel);
				}

				composition.SetCompositingTarget(pAccumulatorWrite);

				if (isFirstPass || isSecondPass)
				{
					if (pCameraComponent->_pSkyBox != nullptr)
					{
						composition.AddModel(pCameraComponent->_pSkyBox->_model);
					}
					composition.Clear();
				}
			}
		}

		void PRERendering::ShadowPassOnRender(
			RenderCompositingNode::RenderComposition& composition,
			void* vShadowPassContext
		)
		{
			auto& shadowPassContext = *static_cast<PREShadowRenderPassContext*>(vShadowPassContext);

			int tag;
			if (shadowPassContext._pPointLightComponent != nullptr)
			{
				tag = shadowPassContext._pPointLightComponent->_tag;
			}
			else if (shadowPassContext._pSpotLightComponent != nullptr)
			{
				tag = shadowPassContext._pSpotLightComponent->_tag;
			}
			else
			{
#ifdef __PRE_DEBUG__
				assert(shadowPassContext._pDirectionalLightComponent != nullptr);
#endif
				tag = shadowPassContext._pDirectionalLightComponent->_tag;
			}

			unordered_set<PREModelRendererComponent*>* pModels = nullptr;
			auto itModels = shadowPassContext._modelTagMap.find(tag);
			if (itModels == shadowPassContext._modelTagMap.end())
			{
				return;
			}

			pModels = &itModels->second;
			if (shadowPassContext._pSpotLightComponent != nullptr)
			{
				auto pSpotLightComponent = shadowPassContext._pSpotLightComponent;

#ifdef __PRE_DEBUG__
				assert(pSpotLightComponent->_pTransform != nullptr);
#endif

				shadowPassContext._lightPOVCamera.SetKind(RenderCamera::Kind::PERSPECTIVE);
				shadowPassContext._lightPOVCamera.SetSize(90);
				shadowPassContext._lightPOVCamera.SetAspectRatio(1);
				shadowPassContext._lightPOVCamera.SetNearClippingPlane(0.1f);
				shadowPassContext._lightPOVCamera.SetFarClippingPlane(10.0f);

				// May cause questions, but you really shouldn't be
				// scaling lights anyhow; eliminate scale
				if (pSpotLightComponent->_pTransform->GetScale() != glm::vec3(1.0f))
				{
					pSpotLightComponent->_pTransform->SetScale(glm::vec3(1.0f));
				}
				shadowPassContext._lightPOVCamera.SetViewMatrix(
					pSpotLightComponent->_pTransform->GetInverseMatrix()
				);
				composition.SetCamera(&shadowPassContext._lightPOVCamera);

				for (auto it = pModels->begin(); it != pModels->end(); ++it)
				{
					auto& modelRendererComponent = **it;

					if (
						modelRendererComponent._pModel == nullptr ||
						modelRendererComponent._pMaterial == nullptr ||
						modelRendererComponent._pMaterial->_pShader == nullptr
						)
					{
						continue;
					}

					composition.AddModel(*modelRendererComponent._pModel);
				}

				composition.Clear();
				composition.SetCompositingTarget(&shadowPassContext._target);
			}
			else if (shadowPassContext._pDirectionalLightComponent != nullptr)
			{
				auto pDirectionalLightComponent = shadowPassContext._pDirectionalLightComponent;

#ifdef __PRE_DEBUG__
				assert(pDirectionalLightComponent->_pTransform != nullptr);
#endif

				shadowPassContext._lightPOVCamera.SetKind(RenderCamera::Kind::ORTHOGRAPHIC);
				shadowPassContext._lightPOVCamera.SetSize(6.0f);
				shadowPassContext._lightPOVCamera.SetAspectRatio(1);
				shadowPassContext._lightPOVCamera.SetNearClippingPlane(-10.0f);
				shadowPassContext._lightPOVCamera.SetFarClippingPlane(10.0f);

				// May cause questions, but you really shouldn't be
				// scaling lights anyhow; eliminate scale
				if (pDirectionalLightComponent->_pTransform->GetScale() != glm::vec3(1.0f))
				{
					pDirectionalLightComponent->_pTransform->SetScale(glm::vec3(1.0f));
				}
				shadowPassContext._lightPOVCamera.SetViewMatrix(
					pDirectionalLightComponent->_pTransform->GetInverseMatrix()
				);
				composition.SetCamera(&shadowPassContext._lightPOVCamera);

				for (auto it = pModels->begin(); it != pModels->end(); ++it)
				{
					auto& modelRendererComponent = **it;

					if (
						modelRendererComponent._pModel == nullptr ||
						modelRendererComponent._pMaterial == nullptr ||
						modelRendererComponent._pMaterial->_pShader == nullptr
						)
					{
						continue;
					}

					composition.AddModel(*modelRendererComponent._pModel);
				}

				composition.Clear();
				composition.SetCompositingTarget(&shadowPassContext._target);
			}
		}

		const unsigned int PRERendering::SHADOW_MAP_SIZE = 1024;

		PRERendering& PRERendering::MakePRERendering(
			const PRERenderingConfig& renderingConfig,
			PREApplicationContext& applicationContext
		)
		{
			auto& renderer = Renderer::MakeRenderer(
				renderingConfig.windowTitle,
				renderingConfig.windowWidth,
				renderingConfig.windowHeight
			);
			return *(new PRERendering(applicationContext, renderer));
		}

		PRERendering::PRERendering(
			PREApplicationContext& applicationContext,
			Renderer& renderer
		)
			:
			_impl(Impl::MakeImpl(applicationContext, renderer, *this)),
			_screenRenderTexture(
				CreateRenderTexture(
					DEFAULT_LAYER,
					renderer.width,
					renderer.height
				)
			) {}

		PRERendering::~PRERendering()
		{
			DestroyRenderTexture(_screenRenderTexture);
			delete &_impl;
		}

		void PRERendering::Initialize() {}

		void PRERendering::Update()
		{
			Renderer::SetActiveRenderer(_impl.renderer);
			for (auto it = _impl.renderPasses.begin(); it != _impl.renderPasses.end(); ++it)
			{
				(*it)->_passCounter = 0;
			}
			_impl.renderer.Update();
		}

		void PRERendering::Shutdown() {}

		RenderCompositingNode& PRERendering::AllocateCompositingNode(
			RenderCompositingNode::OnRender onRender,
			PRERenderTexture& renderTexture
		)
		{
			return _impl.renderer.AllocateCompositingNode(onRender, &renderTexture);
		}

		void PRERendering::DeallocateCompositingNode(
			RenderCompositingNode& compositingNode
		)
		{
			_impl.renderer.DeallocateCompositingNode(compositingNode);
		}

		RenderCamera& PRERendering::AllocateCamera(
			const CameraKind& kind,
			float size,
			float aspectRatio,
			float nearClippingPlane,
			float farClippingPlane
		)
		{
			auto& camera = _impl.renderer.AllocateCamera(
				kind == CameraKind::ORTHOGRAPHIC ?
					Renderer::CameraKind::ORTHOGRAPHIC :
					Renderer::CameraKind::PERSPECTIVE,
				size,
				aspectRatio,
				nearClippingPlane,
				farClippingPlane
			);
			return camera;
		}

		void PRERendering::DeallocateCamera(RenderCamera& camera)
		{
			_impl.renderer.DeallocateCamera(camera);
		}

		void PRERendering::LinkCameraComponentToRenderTexture(
			PRECameraComponent& cameraComponent,
			PRERenderTexture& renderTexture
		)
		{
#ifdef __PRE_DEBUG__
			assert(cameraComponent._pRenderTexture == nullptr);
			assert(renderTexture._pAssociatedCameraComponent == nullptr);
#endif

			cameraComponent._pRenderTexture = &renderTexture;
			renderTexture._pAssociatedCameraComponent = &cameraComponent;
		}

		void PRERendering::UnlinkCameraComponentFromRenderTexture(
			PRECameraComponent& cameraComponent,
			PRERenderTexture& renderTexture
		)
		{
#ifdef __PRE_DEBUG__
			assert(cameraComponent._pRenderTexture == &renderTexture);
			assert(renderTexture._pAssociatedCameraComponent == &cameraComponent);
#endif

			cameraComponent._pRenderTexture = nullptr;
			renderTexture._pAssociatedCameraComponent = nullptr;
		}

		RenderModel& PRERendering::AllocateModel()
		{
			return _impl.renderer.AllocateModel();
		}

		void PRERendering::DeallocateModel(RenderModel& model)
		{
			_impl.renderer.DeallocateModel(model);
		}

		void PRERendering::UpdateModelRendererComponentModel(
			PREModelRendererComponent& modelRendererComponent
		)
		{
			_impl.renderer.SetModelMesh(
				*modelRendererComponent._pModel,
				modelRendererComponent._pMesh != nullptr ?
				&modelRendererComponent._pMesh->_mesh :
				nullptr
			);

			_impl.renderer.SetModelSkeleton(
				*modelRendererComponent._pModel,
				modelRendererComponent._pSkeleton != nullptr ?
				&modelRendererComponent._pSkeleton->_skeleton :
				nullptr
			);

			_impl.renderer.SetModelMaterial(
				*modelRendererComponent._pModel,
				modelRendererComponent._pMaterial != nullptr ?
				&modelRendererComponent._pMaterial->_material :
				nullptr
			);
		}

		void PRERendering::InitializeModelRendererComponentTag(
			PREModelRendererComponent& modelRendererComponent
		)
		{
			auto itTagZero = _impl.modelTagMap.find(0);

#ifdef __PRE_DEBUG__
			assert(itTagZero->second.find(&modelRendererComponent) == itTagZero->second.end());
#endif

			itTagZero->second.insert(&modelRendererComponent);
		}

		void PRERendering::UninitializeModelRendererComponentTag(
			PREModelRendererComponent& modelRendererComponent
		)
		{
			auto itTag = _impl.modelTagMap.find(modelRendererComponent._tag);

#ifdef __PRE_DEBUG__
			assert(itTag != _impl.modelTagMap.end());
			assert(itTag->second.find(&modelRendererComponent) != itTag->second.end());
#endif

			itTag->second.erase(&modelRendererComponent);
			if (itTag->second.empty())
			{
				_impl.modelTagMap.erase(itTag);
			}

			modelRendererComponent._tag = 0;
		}

		void PRERendering::UpdateModelRendererComponentTag(
			PREModelRendererComponent& modelRendererComponent,
			int tag
		)
		{
			UninitializeModelRendererComponentTag(modelRendererComponent);

			auto itNewTag = _impl.modelTagMap.find(tag);
			if (itNewTag == _impl.modelTagMap.end())
			{
				itNewTag = _impl.modelTagMap.insert(
					std::make_pair(
						tag,
						std::move(unordered_set<PREModelRendererComponent*>())
					)
				).first;
			}

#ifdef __PRE_DEBUG__
			assert(itNewTag->second.find(&modelRendererComponent) == itNewTag->second.end());
#endif

			itNewTag->second.insert(&modelRendererComponent);

			modelRendererComponent._tag = tag;
		}

		void PRERendering::LinkCameraComponentToSkyBox(
			PRECameraComponent& cameraComponent,
			PRESkyBox& skyBox
		)
		{
#ifdef __PRE_DEBUG__
			assert(cameraComponent._pSkyBox == nullptr);
			assert(skyBox._associatedCameraComponents.find(&cameraComponent) == skyBox._associatedCameraComponents.end());
#endif

			cameraComponent._pSkyBox = &skyBox;
			skyBox._associatedCameraComponents.insert(&cameraComponent);
		}

		void PRERendering::UnlinkCameraComponentFromSkyBox(
			PRECameraComponent& cameraComponent,
			PRESkyBox& skyBox
		)
		{
#ifdef __PRE_DEBUG__
			assert(cameraComponent._pSkyBox == &skyBox);
			assert(skyBox._associatedCameraComponents.find(&cameraComponent) != skyBox._associatedCameraComponents.end());
#endif

			cameraComponent._pSkyBox = nullptr;
			skyBox._associatedCameraComponents.erase(&cameraComponent);
		}

#pragma region Ambient Light
		void PRERendering::LinkLightToRenderTargets(
			PREAmbientLightComponent& ambientLightComponent
		)
		{
			_impl.UnlinkCompositingChains();

			auto pLightComponent = &ambientLightComponent;
			for (auto it = _impl.renderPasses.begin(); it != _impl.renderPasses.end(); ++it)
			{
				auto pRenderPass = *it;
				auto& compositingChain = _impl.compositingChains.at(pRenderPass->_layer);
				pLightComponent->_fronts[pRenderPass->_layer] = compositingChain.begin();

				auto pLightContext = new PRELightRenderPassContext(
					_impl.modelTagMap,
					*pRenderPass,
					ambientLightComponent
				);
				auto pNewLightData = new PRELightRenderPassData(
					_impl.renderer.AllocateCompositingNode(
						LightPassOnRender,
						pLightContext
					),
					*pLightContext
				);

				_impl.LinkLightToRenderTarget(
					*pRenderPass,
					*pNewLightData,
					pLightComponent,
					compositingChain,
					ambientLightComponent._fronts.at(pRenderPass->_layer),
					nullptr
				);
			}

			_impl.ambientLights.insert(&ambientLightComponent);

			_impl.RelinkCompositingChains();
		}

		void PRERendering::UnlinkLightFromRenderTargets(
			PREAmbientLightComponent& ambientLightComponent
		)
		{
			_impl.UnlinkCompositingChains();

			auto pLightComponent = &ambientLightComponent;
			for (auto it = _impl.renderPasses.begin(); it != _impl.renderPasses.end(); ++it)
			{
				auto pRenderPass = *it;

#ifdef __PRE_DEBUG__
				assert(pRenderPass->_lightMap.find(pLightComponent) != pRenderPass->_lightMap.end());
#endif

				auto itRemovedLightData(pRenderPass->_lightMap.find(pLightComponent)->second);
				auto pRemovedLightData = *itRemovedLightData;

				_impl.UnlinkLightFromRenderTarget(
					*pRenderPass,
					*pRemovedLightData,
					pLightComponent,
					_impl.compositingChains.at(pRenderPass->_layer),
					ambientLightComponent._fronts.at(pRenderPass->_layer),
					nullptr
				);

				_impl.renderer.DeallocateCompositingNode(*pRemovedLightData->pNode);
				delete pRemovedLightData->pRenderPassContext;
				delete pRemovedLightData;
			}
			_impl.ambientLights.erase(&ambientLightComponent);

			_impl.RelinkCompositingChains();
		}
#pragma endregion

#pragma region Point Light
		void PRERendering::LinkLightToRenderTargets(
			PREPointLightComponent& pointLightComponent
		)
		{
			_impl.UnlinkCompositingChains();

			auto pLightComponent = &pointLightComponent;
			for (auto it = _impl.renderPasses.begin(); it != _impl.renderPasses.end(); ++it)
			{
				auto pRenderPass = *it;
				auto& compositingChain = _impl.compositingChains.at(pRenderPass->_layer);
				pLightComponent->_fronts[pRenderPass->_layer] = compositingChain.begin();

				auto pLightContext = new PRELightRenderPassContext(
					_impl.modelTagMap,
					*pRenderPass,
					_impl.shadowMap2D, // TODO: shadowMap3D
					pointLightComponent
				);
				auto pNewLightData = new PRELightRenderPassData(
					_impl.renderer.AllocateCompositingNode(
						LightPassOnRender,
						pLightContext
					),
					*pLightContext
				);

				_impl.LinkLightToRenderTarget(
					*pRenderPass,
					*pNewLightData,
					pLightComponent,
					compositingChain,
					pointLightComponent._fronts.at(pRenderPass->_layer),
					nullptr // TODO: Point Shadows
				);
			}

			_impl.pointLights.insert(&pointLightComponent);

			_impl.RelinkCompositingChains();
		}

		void PRERendering::UnlinkLightFromRenderTargets(
			PREPointLightComponent& pointLightComponent
		)
		{
			_impl.UnlinkCompositingChains();

			auto pLightComponent = &pointLightComponent;
			for (auto it = _impl.renderPasses.begin(); it != _impl.renderPasses.end(); ++it)
			{
				auto pRenderPass = *it;

#ifdef __PRE_DEBUG__
				assert(pRenderPass->_lightMap.find(pLightComponent) != pRenderPass->_lightMap.end());
#endif

				auto itRemovedLightData(pRenderPass->_lightMap.find(pLightComponent)->second);
				auto pRemovedLightData = *itRemovedLightData;

				_impl.UnlinkLightFromRenderTarget(
					*pRenderPass,
					*pRemovedLightData,
					pLightComponent,
					_impl.compositingChains.at(pRenderPass->_layer),
					pointLightComponent._fronts.at(pRenderPass->_layer),
					nullptr // TODO: Point Shadows
				);

				_impl.renderer.DeallocateCompositingNode(*pRemovedLightData->pNode);
				delete pRemovedLightData->pRenderPassContext;
				delete pRemovedLightData;
			}

			_impl.pointLights.erase(&pointLightComponent);

			_impl.RelinkCompositingChains();
		}
#pragma endregion

#pragma region Spot Light
		void PRERendering::LinkLightToRenderTargets(
			PRESpotLightComponent& spotLightComponent
		)
		{
			_impl.UnlinkCompositingChains();

			auto pLightComponent = &spotLightComponent;
			auto& shadowPassData = spotLightComponent._shadowPassData;
			for (auto it = _impl.renderPasses.begin(); it != _impl.renderPasses.end(); ++it)
			{
				auto pRenderPass = *it;
				auto& compositingChain = _impl.compositingChains.at(pRenderPass->_layer);
				pLightComponent->_fronts[pRenderPass->_layer] = compositingChain.begin();

				auto pLightContext = new PRELightRenderPassContext(
					_impl.modelTagMap,
					*pRenderPass,
					_impl.shadowMap2D,
					spotLightComponent
				);
				auto pNewLightData = new PRELightRenderPassData(
					_impl.renderer.AllocateCompositingNode(
						LightPassOnRender,
						pLightContext
					),
					*pLightContext
				);

				PREShadowRenderPassData* pNewShadowData = nullptr;
				// link shadow pass if necessary
				if (shadowPassData.find(pRenderPass->_layer) == shadowPassData.end())
				{
					auto pShadowContext = new PREShadowRenderPassContext(
						_impl.lightPOVCamera,
						_impl.modelTagMap,
						_impl.shadowMap2D,
						spotLightComponent
					);
					pNewShadowData = new PREShadowRenderPassData(
						_impl.renderer.AllocateCompositingNode(
							ShadowPassOnRender,
							pShadowContext
						),
						*pShadowContext
					);

					shadowPassData[pRenderPass->_layer] = pNewShadowData;
				}

				_impl.LinkLightToRenderTarget(
					*pRenderPass,
					*pNewLightData,
					pLightComponent,
					compositingChain,
					spotLightComponent._fronts.at(pRenderPass->_layer),
					pNewShadowData
				);
			}

			_impl.spotLights.insert(&spotLightComponent);

			_impl.RelinkCompositingChains();
		}

		void PRERendering::UnlinkLightFromRenderTargets(
			PRESpotLightComponent& spotLightComponent
		)
		{
			_impl.UnlinkCompositingChains();

			auto pLightComponent = &spotLightComponent;
			auto& shadowPassData = spotLightComponent._shadowPassData;
			for (auto it = _impl.renderPasses.begin(); it != _impl.renderPasses.end(); ++it)
			{
				auto pRenderPass = *it;

#ifdef __PRE_DEBUG__
				assert(pRenderPass->_lightMap.find(pLightComponent) != pRenderPass->_lightMap.end());
#endif

				auto itRemovedLightData(pRenderPass->_lightMap.find(pLightComponent)->second);
				auto pRemovedLightData = *itRemovedLightData;

				// unlink shadow node if necessary
				PREShadowRenderPassData* pRemovedShadowData = nullptr;
				auto itRemovedShadowData = shadowPassData.find(pRenderPass->_layer);
				if (itRemovedShadowData != shadowPassData.end())
				{
					pRemovedShadowData = itRemovedShadowData->second;
				}

				_impl.UnlinkLightFromRenderTarget(
					*pRenderPass,
					*pRemovedLightData,
					pLightComponent,
					_impl.compositingChains.at(pRenderPass->_layer),
					spotLightComponent._fronts.at(pRenderPass->_layer),
					pRemovedShadowData
				);

				if (pRemovedShadowData != nullptr)
				{
					_impl.renderer.DeallocateCompositingNode(*pRemovedShadowData->pNode);
					delete pRemovedShadowData->pRenderPassContext;
					delete pRemovedShadowData;
					shadowPassData.erase(itRemovedShadowData);
				}

				_impl.renderer.DeallocateCompositingNode(*pRemovedLightData->pNode);
				delete pRemovedLightData->pRenderPassContext;
				delete pRemovedLightData;
			}

			_impl.spotLights.erase(&spotLightComponent);

			_impl.RelinkCompositingChains();
		}
#pragma endregion

#pragma region Directional Light
		void PRERendering::LinkLightToRenderTargets(
			PREDirectionalLightComponent& directionalLightComponent
		)
		{
			_impl.UnlinkCompositingChains();

			auto pLightComponent = &directionalLightComponent;
			auto& shadowPassData = directionalLightComponent._shadowPassData;
			for (auto it = _impl.renderPasses.begin(); it != _impl.renderPasses.end(); ++it)
			{
				auto pRenderPass = *it;
				auto& compositingChain = _impl.compositingChains.at(pRenderPass->_layer);
				pLightComponent->_fronts[pRenderPass->_layer] = compositingChain.begin();

				auto pLightContext = new PRELightRenderPassContext(
					_impl.modelTagMap,
					*pRenderPass,
					_impl.shadowMap2D,
					directionalLightComponent
				);
				auto pNewLightData = new PRELightRenderPassData(
					_impl.renderer.AllocateCompositingNode(
						LightPassOnRender,
						pLightContext
					),
					*pLightContext
				);

				PREShadowRenderPassData* pNewShadowData = nullptr;
				// link shadow pass if necessary
				if (shadowPassData.find(pRenderPass->_layer) == shadowPassData.end())
				{
					auto pShadowContext = new PREShadowRenderPassContext(
						_impl.lightPOVCamera,
						_impl.modelTagMap,
						_impl.shadowMap2D,
						directionalLightComponent
					);
					pNewShadowData = new PREShadowRenderPassData(
						_impl.renderer.AllocateCompositingNode(
							ShadowPassOnRender,
							pShadowContext
						),
						*pShadowContext
					);

					shadowPassData[pRenderPass->_layer] = pNewShadowData;
				}

				_impl.LinkLightToRenderTarget(
					*pRenderPass,
					*pNewLightData,
					pLightComponent,
					compositingChain,
					directionalLightComponent._fronts.at(pRenderPass->_layer),
					pNewShadowData
				);
			}

			_impl.directionalLights.insert(&directionalLightComponent);

			_impl.RelinkCompositingChains();
		}

		void PRERendering::UnlinkLightFromRenderTargets(
			PREDirectionalLightComponent& directionalLightComponent
		)
		{
			_impl.UnlinkCompositingChains();

			auto pLightComponent = &directionalLightComponent;
			auto& shadowPassData = directionalLightComponent._shadowPassData;
			for (auto it = _impl.renderPasses.begin(); it != _impl.renderPasses.end(); ++it)
			{
				auto pRenderPass = *it;

#ifdef __PRE_DEBUG__
				assert(pRenderPass->_lightMap.find(pLightComponent) != pRenderPass->_lightMap.end());
#endif

				auto itRemovedLightData(pRenderPass->_lightMap.find(pLightComponent)->second);
				auto pRemovedLightData = *itRemovedLightData;

				// unlink shadow node if necessary
				PREShadowRenderPassData* pRemovedShadowData = nullptr;
				auto itRemovedShadowData = shadowPassData.find(pRenderPass->_layer);
				if (itRemovedShadowData != shadowPassData.end())
				{
					pRemovedShadowData = itRemovedShadowData->second;
				}

				_impl.UnlinkLightFromRenderTarget(
					*pRenderPass,
					*pRemovedLightData,
					pLightComponent,
					_impl.compositingChains.at(pRenderPass->_layer),
					directionalLightComponent._fronts.at(pRenderPass->_layer),
					pRemovedShadowData
				);

				if (pRemovedShadowData != nullptr)
				{
					_impl.renderer.DeallocateCompositingNode(*pRemovedShadowData->pNode);
					delete pRemovedShadowData->pRenderPassContext;
					delete pRemovedShadowData;
					shadowPassData.erase(itRemovedShadowData);
				}

				_impl.renderer.DeallocateCompositingNode(*pRemovedLightData->pNode);
				delete pRemovedLightData->pRenderPassContext;
				delete pRemovedLightData;
			}

			_impl.directionalLights.erase(&directionalLightComponent);

			_impl.RelinkCompositingChains();
		}
#pragma endregion

		void PRERendering::LinkRenderTextureToLights(
			PRERenderTexture& renderTexture
		)
		{
			_impl.UnlinkCompositingChains();

			auto pRenderPass = &renderTexture;
			auto& compositingChain = _impl.compositingChains.at(renderTexture._layer);
#pragma region Ambient Lights
			for (auto it = _impl.ambientLights.begin(); it != _impl.ambientLights.end(); ++it)
			{
				auto pLightComponent = *it;
				auto& ambientLightComponent = *pLightComponent;

				auto pLightContext = new PRELightRenderPassContext(
					_impl.modelTagMap,
					*pRenderPass,
					ambientLightComponent
				);
				auto pNewLightData = new PRELightRenderPassData(
					_impl.renderer.AllocateCompositingNode(
						LightPassOnRender,
						pLightContext
					),
					*pLightContext
				);

				_impl.LinkLightToRenderTarget(
					*pRenderPass,
					*pNewLightData,
					pLightComponent,
					compositingChain,
					ambientLightComponent._fronts.at(pRenderPass->_layer),
					nullptr
				);
			}
#pragma endregion

#pragma region Point Lights
			for (auto it = _impl.pointLights.begin(); it != _impl.pointLights.end(); ++it)
			{
				auto pLightComponent = *it;
				auto& pointLightComponent = *pLightComponent;

				auto pLightContext = new PRELightRenderPassContext(
					_impl.modelTagMap,
					*pRenderPass,
					_impl.shadowMap2D, // TODO: shadowMap3D
					pointLightComponent
				);
				auto pNewLightData = new PRELightRenderPassData(
					_impl.renderer.AllocateCompositingNode(
						LightPassOnRender,
						pLightContext
					),
					*pLightContext
				);

				_impl.LinkLightToRenderTarget(
					*pRenderPass,
					*pNewLightData,
					pLightComponent,
					compositingChain,
					pointLightComponent._fronts.at(pRenderPass->_layer),
					nullptr // TODO: Point Shadows
				);
			}
#pragma endregion

#pragma region Spot Lights
			for (auto it = _impl.spotLights.begin(); it != _impl.spotLights.end(); ++it)
			{
				auto pLightComponent = *it;
				auto& spotLightComponent = *pLightComponent;

				auto pLightContext = new PRELightRenderPassContext(
					_impl.modelTagMap,
					*pRenderPass,
					_impl.shadowMap2D,
					spotLightComponent
				);
				auto pNewLightData = new PRELightRenderPassData(
					_impl.renderer.AllocateCompositingNode(
						LightPassOnRender,
						pLightContext
					),
					*pLightContext
				);

				_impl.LinkLightToRenderTarget(
					*pRenderPass,
					*pNewLightData,
					pLightComponent,
					compositingChain,
					spotLightComponent._fronts.at(pRenderPass->_layer),
					spotLightComponent._shadowPassData.at(pRenderPass->_layer)
				);
			}
#pragma endregion

#pragma region Directional Lights
			for (auto it = _impl.directionalLights.begin(); it != _impl.directionalLights.end(); ++it)
			{
				auto pLightComponent = *it;
				auto& directionalLightComponent = *pLightComponent;

				auto pLightContext = new PRELightRenderPassContext(
					_impl.modelTagMap,
					*pRenderPass,
					_impl.shadowMap2D,
					directionalLightComponent
				);
				auto pNewLightData = new PRELightRenderPassData(
					_impl.renderer.AllocateCompositingNode(
						LightPassOnRender,
						pLightContext
					),
					*pLightContext
				);

				_impl.LinkLightToRenderTarget(
					*pRenderPass,
					*pNewLightData,
					pLightComponent,
					compositingChain,
					directionalLightComponent._fronts.at(pRenderPass->_layer),
					nullptr // TODO: Directional Shadows
				);
			}
#pragma endregion

			_impl.renderPasses.insert(pRenderPass);

			_impl.RelinkCompositingChains();
		}

		void PRERendering::UnlinkRenderTextureFromLights(
			PRERenderTexture& renderTexture
		)
		{
			_impl.UnlinkCompositingChains();

			auto pRenderPass = &renderTexture;
			auto& compositingChain = _impl.compositingChains.at(renderTexture._layer);
#pragma region Ambient Lights
			for (auto it = _impl.ambientLights.begin(); it != _impl.ambientLights.end(); ++it)
			{
				auto pLightComponent = *it;
				auto& ambientLightComponent = *pLightComponent;
				auto itRemovedLightData(pRenderPass->_lightMap.find(pLightComponent)->second);
				auto pRemovedLightData = *itRemovedLightData;

				_impl.UnlinkLightFromRenderTarget(
					*pRenderPass,
					*pRemovedLightData,
					pLightComponent,
					compositingChain,
					ambientLightComponent._fronts.at(pRenderPass->_layer),
					nullptr
				);

				_impl.renderer.DeallocateCompositingNode(*pRemovedLightData->pNode);
				delete pRemovedLightData->pRenderPassContext;
				delete pRemovedLightData;
			}
#pragma endregion

#pragma region Point Lights
			for (auto it = _impl.pointLights.begin(); it != _impl.pointLights.end(); ++it)
			{
				auto pLightComponent = *it;
				auto& pointLightComponent = *pLightComponent;
				auto itRemovedLightData(pRenderPass->_lightMap.find(pLightComponent)->second);
				auto pRemovedLightData = *itRemovedLightData;

				_impl.UnlinkLightFromRenderTarget(
					*pRenderPass,
					*pRemovedLightData,
					pLightComponent,
					compositingChain,
					pointLightComponent._fronts.at(pRenderPass->_layer),
					nullptr // TODO: Point Shadows
				);

				_impl.renderer.DeallocateCompositingNode(*pRemovedLightData->pNode);
				delete pRemovedLightData->pRenderPassContext;
				delete pRemovedLightData;
			}
#pragma endregion

#pragma region Spot Lights
			for (auto it = _impl.spotLights.begin(); it != _impl.spotLights.end(); ++it)
			{
				auto pLightComponent = *it;
				auto& spotLightComponent = *pLightComponent;
				auto itRemovedLightData(pRenderPass->_lightMap.find(pLightComponent)->second);
				auto pRemovedLightData = *itRemovedLightData;

				_impl.UnlinkLightFromRenderTarget(
					*pRenderPass,
					*pRemovedLightData,
					pLightComponent,
					compositingChain,
					spotLightComponent._fronts.at(pRenderPass->_layer),
					spotLightComponent._shadowPassData.at(pRenderPass->_layer)
				);

				_impl.renderer.DeallocateCompositingNode(*pRemovedLightData->pNode);
				delete pRemovedLightData->pRenderPassContext;
				delete pRemovedLightData;
			}
#pragma endregion

#pragma region Directional Lights
			for (auto it = _impl.directionalLights.begin(); it != _impl.directionalLights.end(); ++it)
			{
				auto pLightComponent = *it;
				auto& directionalLightComponent = *pLightComponent;
				auto itRemovedLightData(pRenderPass->_lightMap.find(pLightComponent)->second);
				auto pRemovedLightData = *itRemovedLightData;

				_impl.UnlinkLightFromRenderTarget(
					*pRenderPass,
					*pRemovedLightData,
					pLightComponent,
					compositingChain,
					directionalLightComponent._fronts.at(pRenderPass->_layer),
					directionalLightComponent._shadowPassData.at(pRenderPass->_layer)
				);

				_impl.renderer.DeallocateCompositingNode(*pRemovedLightData->pNode);
				delete pRemovedLightData->pRenderPassContext;
				delete pRemovedLightData;
			}
#pragma endregion

			_impl.renderPasses.erase(pRenderPass);

			_impl.RelinkCompositingChains();
		}
	}
}