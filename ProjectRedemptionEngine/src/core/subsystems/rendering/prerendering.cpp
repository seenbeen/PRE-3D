#include <core/subsystems/rendering/prerendering.h>

#include <algorithm>
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
#include <core/components/prepointlightcomponent.h>

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

#ifdef __PRE_DEBUG__
#include <assert.h>
#endif

namespace PRE
{
	namespace Core
	{
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
					screenCamera
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
			RenderCamera& screenCamera
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
			screenCamera(screenCamera)
		{
			compositingChain.push_back(new PRELightRenderPassData(screenCompositingNode));
		}

		PRERendering::Impl::~Impl()
		{
			delete compositingChain.front();
			renderer.DeallocateCamera(screenCamera);
			renderer.DeallocateModel(screenModel);
			renderer.DeallocateMaterial(screenMaterial);
			renderer.DeallocateMesh(screenMesh);
			renderer.DeallocateShaderProgram(screenShaderProgram);
			renderer.DeallocateCompositingNode(screenCompositingNode);
			Renderer::ShutdownRenderer(renderer);
		}


		void PRERendering::Impl::LinkLightToRenderTarget(
			PRERenderTexture& renderPass,
			PRELightRenderPassData& lightData,
			unordered_map<PRERenderTexture*, list<PRELightRenderPassData*>::iterator>& lightPassMap
		)
		{
			auto pRenderPass = &renderPass;
			auto pLightData = &lightData;

			auto itPreviousPassLastLightData(pRenderPass->_front);
			PRELightRenderPassData* pPreviousPassLastLightData = *(--itPreviousPassLastLightData);

			PRELightRenderPassData* pCurrentPassFirstLightData = nullptr;
			if (pRenderPass->_front != compositingChain.end())
			{
				pCurrentPassFirstLightData = *pRenderPass->_front;
			}

			// detach next light if exists
			if (pCurrentPassFirstLightData != nullptr)
			{
				renderer.DetachCompositingNodeDependency(
					*pPreviousPassLastLightData->pNode,
					*pCurrentPassFirstLightData->pNode
				);
			}

			// splice in new light
			renderer.AttachCompositingNodeDependency(
				*pPreviousPassLastLightData->pNode,
				*pLightData->pNode
			);

			// re-attach next light if exists
			if (pCurrentPassFirstLightData != nullptr)
			{
				renderer.AttachCompositingNodeDependency(
					*pLightData->pNode,
					*pCurrentPassFirstLightData->pNode
				);
			}

			// insert light render pass data
			pRenderPass->_front = compositingChain.insert(pRenderPass->_front, pLightData);

			// link point light

#ifdef __PRE_DEBUG__
			assert(lightPassMap.find(pRenderPass) == lightPassMap.end());
#endif

			lightPassMap[pRenderPass] = pRenderPass->_front;
		}

		void PRERendering::Impl::UnlinkLightFromRenderTarget(
			PRERenderTexture& renderPass,
			list<PRELightRenderPassData*>::iterator itRemovedLightData,
			unordered_map<PRERenderTexture*, list<PRELightRenderPassData*>::iterator>& lightPassMap
		)
		{
			auto pRenderPass = &renderPass;

			auto pRemovedLightData = *itRemovedLightData;

			auto itPreviousLightData(itRemovedLightData);
			PRELightRenderPassData* pPreviousLightData = *(--itPreviousLightData);

			PRELightRenderPassData* pNextLightData = nullptr;
			auto itNextLightData(itRemovedLightData);
			++itNextLightData;
			if (itNextLightData != compositingChain.end())
			{
				pNextLightData = *itNextLightData;
			}

			// unlink previous light
			renderer.DetachCompositingNodeDependency(
				*pPreviousLightData->pNode,
				*pRemovedLightData->pNode
			);

			// unlink next light if exists
			if (pNextLightData != nullptr)
			{
				renderer.DetachCompositingNodeDependency(
					*pRemovedLightData->pNode,
					*pNextLightData->pNode
				);
			}

			// re-splice if next exists
			if (pNextLightData != nullptr)
			{
				renderer.AttachCompositingNodeDependency(
					*pPreviousLightData->pNode,
					*pNextLightData->pNode
				);
			}

			// erase light render pass data
			if (itRemovedLightData == pRenderPass->_front)
			{
				++pRenderPass->_front;
			}
			compositingChain.erase(itRemovedLightData);

#ifdef __PRE_DEBUG__
			assert(lightPassMap.find(pRenderPass) != lightPassMap.end());
#endif

			lightPassMap.erase(pRenderPass);
		}

		PRERenderTexture& PRERendering::GetScreenRenderTexture()
		{
			return _screenRenderTexture;
		}

		PRERenderTexture& PRERendering::CreateRenderTexture(unsigned int width, unsigned int height)
		{
			auto pRenderTexture = new PRERenderTexture(
				_impl.renderer.AllocateCompositingTarget(width, height),
				_impl.renderer.AllocateCompositingTarget(width, height),
				_impl.compositingChain.end()
			);
			LinkRenderTextureToLights(*pRenderTexture);
			return *pRenderTexture;
		}

		void PRERendering::DestroyRenderTexture(PRERenderTexture& renderTexture)
		{
			auto pRenderTexture = &renderTexture;
			UnlinkRenderTextureFromLights(*pRenderTexture);
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
			return *pShader;
		}

		void PRERendering::DestroyShader(PREShader& shader)
		{
			_impl.renderer.DeallocateShaderProgram(shader._shaderProgram);
			delete &shader;
			_impl.applicationContext.assetManager.TryFreeShader(shader);
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
			delete &texture;
			_impl.applicationContext.assetManager.TryFreeTexture(texture);
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
			delete &mesh;
			_impl.applicationContext.assetManager.TryFreeMesh(mesh);
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
			delete &skeleton;
			_impl.applicationContext.assetManager.TryFreeSkeleton(skeleton);
		}

		PREAnimation& PRERendering::CreateAnimation(
			const PREAnimationConfig& animationConfig
		)
		{
			return *(new PREAnimation(animationConfig));
		}

		void PRERendering::DestroyAnimation(PREAnimation& animation)
		{
			delete &animation;
			_impl.applicationContext.assetManager.TryFreeAnimation(animation);
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
				composition.SetCamera(pCameraComponent->_pCamera);

				auto pAccumulatorWrite = lightPassContext._renderTexture._accumulatorBufferIsA ?
					lightPassContext._renderTexture._pBufferA :
					lightPassContext._renderTexture._pBufferB;

				auto pAccumulatorRead = lightPassContext._renderTexture._accumulatorBufferIsA ?
					lightPassContext._renderTexture._pBufferB :
					lightPassContext._renderTexture._pBufferA;

				lightPassContext._renderTexture._accumulatorBufferIsA = !lightPassContext._renderTexture._accumulatorBufferIsA;

				// TODO: Spatial query optimizing to only render visible models
				for (
					auto it = pCameraComponent->_associatedModelComponents.begin();
					it != pCameraComponent->_associatedModelComponents.end();
					++it
					)
				{
					auto& modelRendererComponent = **it;
					modelRendererComponent.AllocateIfNotAllocated();
					if (modelRendererComponent._pMaterial != nullptr)
					{
						// TODO: switch on light type
						if (modelRendererComponent._pMaterial->_pShader != nullptr)
						{
							modelRendererComponent._pMaterial->_material.SetTextureBinding(
								pAccumulatorRead,
								PREShader::LIGHT_ACCUMULATOR_BINDING
							);
							modelRendererComponent._pMaterial->BindRenderTextureAccumulatorBindings();

							auto pShader = modelRendererComponent._pMaterial->_pShader;
							pShader->SetInt(
								PREShader::IS_FIRST_LIGHT_PASS,
								lightPassContext._isFirstPass ? 1 : 0
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
							if (lightPassContext._pPointLightComponent != nullptr)
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
							// else if
						}
					}
					composition.AddModel(*modelRendererComponent._pModel);
				}

				if (pCameraComponent->_pSkyBox != nullptr)
				{
					composition.AddModel(pCameraComponent->_pSkyBox->_model);
				}

				composition.SetCompositingTarget(pAccumulatorWrite);

				// TODO: take advantage of depth buffer early z test
				// by setting depth test leq and not clearing depth buffer
				// unless first pass.
				composition.Clear();
			}
		}

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
			_screenRenderTexture(CreateRenderTexture(renderer.width, renderer.height)) {}

		PRERendering::~PRERendering()
		{
			DestroyRenderTexture(_screenRenderTexture);
			delete &_impl;
		}

		void PRERendering::Initialize() {}

		void PRERendering::Update()
		{
			Renderer::SetActiveRenderer(_impl.renderer);
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

		void PRERendering::LinkModelRendererComponentToCameraComponent(
			PREModelRendererComponent& modelRendererComponent,
			PRECameraComponent& cameraComponent
		)
		{
#ifdef __PRE_DEBUG__
			assert(modelRendererComponent._pCameraComponent == nullptr);
			assert(cameraComponent._associatedModelComponents.find(&modelRendererComponent) == cameraComponent._associatedModelComponents.end());
#endif

			modelRendererComponent._pCameraComponent = &cameraComponent;
			cameraComponent._associatedModelComponents.insert(&modelRendererComponent);
		}

		void PRERendering::UnlinkModelRendererComponentFromCameraComponent(
			PREModelRendererComponent& modelRendererComponent,
			PRECameraComponent& cameraComponent
		)
		{
#ifdef __PRE_DEBUG__
			assert(modelRendererComponent._pCameraComponent == &cameraComponent);
			assert(cameraComponent._associatedModelComponents.find(&modelRendererComponent) != cameraComponent._associatedModelComponents.end());
#endif

			modelRendererComponent._pCameraComponent = nullptr;
			cameraComponent._associatedModelComponents.erase(&modelRendererComponent);
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

		void PRERendering::LinkLightToRenderTargets(
			PREPointLightComponent& pointLightComponent
		)
		{
			for (auto it = _impl.renderPasses.begin(); it != _impl.renderPasses.end(); ++it)
			{
				auto pRenderPass = *it;

				auto pLightContext = new PRELightRenderPassContext(
					pRenderPass->_front == _impl.compositingChain.end(),
					*pRenderPass,
					pointLightComponent
				);
				auto pNewLightData = new PRELightRenderPassData(
					_impl.renderer.AllocateCompositingNode(
						LightPassOnRender,
						pLightContext
					),
					*pLightContext
				);

				// attach shadow pass

				_impl.LinkLightToRenderTarget(
					*pRenderPass,
					*pNewLightData,
					pointLightComponent._passMap
				);
			}

#ifdef __PRE_DEBUG__
			assert(pointLightComponent._passMap.size() == _impl.renderPasses.size());
#endif

			_impl.pointLights.insert(&pointLightComponent);
		}

		void PRERendering::UnlinkLightFromRenderTargets(
			PREPointLightComponent& pointLightComponent
		)
		{
			for (auto it = _impl.renderPasses.begin(); it != _impl.renderPasses.end(); ++it)
			{
				auto pRenderPass = *it;
				auto itRemovedLightData(pointLightComponent._passMap.find(pRenderPass)->second);
				auto pRemovedLightData = *itRemovedLightData;

				_impl.UnlinkLightFromRenderTarget(
					*pRenderPass,
					itRemovedLightData,
					pointLightComponent._passMap
				);

				// detach shadow pass

				_impl.renderer.DeallocateCompositingNode(*pRemovedLightData->pNode);
				delete pRemovedLightData->pRenderPassContext;
				delete pRemovedLightData;
			}

#ifdef __PRE_DEBUG__
			assert(pointLightComponent._passMap.empty());
#endif

			_impl.pointLights.erase(&pointLightComponent);
		}

		void PRERendering::LinkRenderTextureToLights(
			PRERenderTexture& renderTexture
		)
		{
			auto pRenderPass = &renderTexture;
			for (auto it = _impl.pointLights.begin(); it != _impl.pointLights.end(); ++it)
			{
				auto& pointLightComponent = **it;

				auto pLightContext = new PRELightRenderPassContext(
					pRenderPass->_front == _impl.compositingChain.end(),
					*pRenderPass,
					pointLightComponent
				);
				auto pNewLightData = new PRELightRenderPassData(
					_impl.renderer.AllocateCompositingNode(
						LightPassOnRender,
						pLightContext
					),
					*pLightContext
				);

				// attach shadow pass

				_impl.LinkLightToRenderTarget(
					*pRenderPass,
					*pNewLightData,
					pointLightComponent._passMap
				);
			}

			_impl.renderPasses.push_back(pRenderPass);
		}

		void PRERendering::UnlinkRenderTextureFromLights(
			PRERenderTexture& renderTexture
		)
		{
			auto pRenderPass = &renderTexture;
			for (auto it = _impl.pointLights.begin(); it != _impl.pointLights.end(); ++it)
			{
				auto& pointLightComponent = **it;
				auto itRemovedLightData(pointLightComponent._passMap.find(pRenderPass)->second);
				auto pRemovedLightData = *itRemovedLightData;

				_impl.UnlinkLightFromRenderTarget(
					*pRenderPass,
					itRemovedLightData,
					pointLightComponent._passMap
				);

				// detach shadow pass

				_impl.renderer.DeallocateCompositingNode(*pRemovedLightData->pNode);
				delete pRemovedLightData->pRenderPassContext;
				delete pRemovedLightData;
			}

			_impl.renderPasses.erase(
				std::find(
					_impl.renderPasses.begin(),
					_impl.renderPasses.end(),
					pRenderPass
				)
			);
		}
	}
}