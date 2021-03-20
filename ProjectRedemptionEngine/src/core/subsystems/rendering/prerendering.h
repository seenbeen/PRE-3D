#pragma once
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <glad/glad.h>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		struct PRERenderingConfig;

		class PREApplicationContext;

		class PREModelRendererComponent;
		class PRECameraComponent;
		class PREAmbientLightComponent;
		class PREPointLightComponent;
		class PRESpotLightComponent;
		class PREDirectionalLightComponent;

		class PRERenderTexture;
		class PREShader;
		class PREMesh;
		class PRESkeleton;
		class PREBoneConfig;
		class PRETexture;
		class PREMaterial;

		class PRELightRenderPassContext;
		class PRELightRenderPassData;
		class PREShadowRenderPassContext;
		class PREShadowRenderPassData;

		class PREAnimation;
		class PREAnimationConfig;
		class PREAnimator;
		class PREAnimatorConfig;

		class PRESkyBox;

		using std::list;
		using std::map;
		using std::string;
		using std::unordered_map;
		using std::unordered_set;

		using PRE::RenderingModule::Renderer;
		using PRE::RenderingModule::RenderCompositingNode;
		using PRE::RenderingModule::RenderCompositingTarget;
		using PRE::RenderingModule::RenderCamera;
		using PRE::RenderingModule::RenderShaderProgram;
		using PRE::RenderingModule::RenderMesh;
		using PRE::RenderingModule::RenderMaterial;
		using PRE::RenderingModule::RenderModel;

		class PRERendering
		{
			PRERendering& operator=(const PRERendering&) = delete;
			PRERendering(const PRERendering&) = delete;

			friend class PREApplicationContext;
			friend class PREModelRendererComponent;
			friend class PRECameraComponent;
			friend class PRERenderTexture;

			friend class PREAmbientLightComponent;
			friend class PREPointLightComponent;
			friend class PRESpotLightComponent;
			friend class PREDirectionalLightComponent;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class PRERendering;

			private:
				static Impl& MakeImpl(
					PREApplicationContext& applicationContext,
					Renderer& renderer,
					PRERendering& rendering
				);

				static void ScreenOnRender(
					RenderCompositingNode::RenderComposition& composition,
					void* vRenderingImpl
				);

				PREApplicationContext& applicationContext;
				Renderer& renderer;
				PRERendering& rendering;
				RenderCompositingNode& screenCompositingNode;
				RenderShaderProgram& screenShaderProgram;
				RenderMesh& screenMesh;
				RenderMaterial& screenMaterial;
				RenderModel& screenModel;
				RenderCamera& screenCamera;

				PRELightRenderPassData* const rootRenderPassData;

				RenderCompositingTarget& shadowMap2D;
				//RenderCompositingTarget& shadowMap3D;

				unordered_map<int, unordered_set<PREModelRendererComponent*>> modelTagMap;

				map<int, list<PRELightRenderPassData*>> compositingChains;

				unordered_set<PRERenderTexture*> renderPasses;
				unordered_set<PREAmbientLightComponent*> ambientLights;
				unordered_set<PREPointLightComponent*> pointLights;
				unordered_set<PRESpotLightComponent*> spotLights;
				unordered_set<PREDirectionalLightComponent*> directionalLights;

				Impl(
					PREApplicationContext& applicationContext,
					Renderer& renderer,
					PRERendering& rendering,
					RenderShaderProgram& screenShaderProgram,
					RenderMesh& screenMesh,
					RenderMaterial& screenMaterial,
					RenderModel& screenModel,
					RenderCamera& screenCamera
				);
				~Impl();

				void UnlinkCompositingChains();
				void RelinkCompositingChains();

				void LinkLightToRenderTarget(
					PRERenderTexture& renderPass,
					PRELightRenderPassData& lightData,
					void* pLightComponent,
					list<PRELightRenderPassData*>& compositingChain,
					list<PRELightRenderPassData*>::iterator& itLightFront
				);

				void UnlinkLightFromRenderTarget(
					PRERenderTexture& renderPass,
					PRELightRenderPassData& lightData,
					void* pLightComponent,
					list<PRELightRenderPassData*>& compositingChain,
					list<PRELightRenderPassData*>::iterator& itLightFront
				);
			};

		public:
			static const int DEFAULT_LAYER;

			PRERenderTexture& GetScreenRenderTexture();

			PRERenderTexture& CreateRenderTexture(int layer, unsigned int width, unsigned int height);
			void DestroyRenderTexture(PRERenderTexture& renderTexture);

			PREShader& CreateShader(const string& vertex, const string& fragment);
			void DestroyShader(PREShader& shader);

			PRETexture& CreateTexture(
				unsigned int width,
				unsigned int height,
				const unsigned char* data
			);
			void DestroyTexture(PRETexture& texture);

			PREMaterial& CreateMaterial();
			void DestroyMaterial(PREMaterial& material);

			PREMesh& CreateMesh(
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
			void DestroyMesh(PREMesh& mesh);

			PRESkeleton& CreateSkeleton(
				const PREBoneConfig& rootBoneConfig
			);
			void DestroySkeleton(PRESkeleton& skeleton);

			PREAnimation& CreateAnimation(
				const PREAnimationConfig& animationConfig
			);
			void DestroyAnimation(PREAnimation& animation);

			PREAnimator& CreateAnimator(
				const PREAnimatorConfig& animatorConfig
			);
			void DestroyAnimator(PREAnimator& animator);

			PRESkyBox& CreateSkyBox(
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
			);
			void DestroySkyBox(PRESkyBox& skyBox);

		private:
			static const unsigned int SHADOW_MAP_SIZE;

			enum class CameraKind { ORTHOGRAPHIC, PERSPECTIVE };

			static PRERendering& MakePRERendering(
				const PRERenderingConfig& renderingConfig,
				PREApplicationContext& applicationContext
			);

			static void LightPassOnRender(
				RenderCompositingNode::RenderComposition& composition,
				void* vLightPassContext
			);

			static void ShadowPassOnRender(
				RenderCompositingNode::RenderComposition& composition,
				void* vShadowPassContext
			);

			Impl& _impl;
			PRERenderTexture& _screenRenderTexture;

			PRERendering(
				PREApplicationContext& applicationContext,
				Renderer& renderer
			);
			~PRERendering();

			void Initialize();
			void Update();
			void Shutdown();

			RenderCompositingNode& AllocateCompositingNode(
				RenderCompositingNode::OnRender onRender,
				PRERenderTexture& renderTexture
			);
			void DeallocateCompositingNode(
				RenderCompositingNode& compositingNode
			);

			RenderCamera& AllocateCamera(
				const CameraKind& kind,
				float size,
				float aspectRatio,
				float nearClippingPlane,
				float farClippingPlane
			);
			void DeallocateCamera(RenderCamera& camera);

			void LinkCameraComponentToRenderTexture(
				PRECameraComponent& cameraComponent,
				PRERenderTexture& renderTexture
			);
			void UnlinkCameraComponentFromRenderTexture(
				PRECameraComponent& cameraComponent,
				PRERenderTexture& renderTexture
			);

			RenderModel& AllocateModel();
			void DeallocateModel(RenderModel& model);

			void UpdateModelRendererComponentModel(
				PREModelRendererComponent& modelRendererComponent
			);

			void InitializeModelRendererComponentTag(
				PREModelRendererComponent& modelRendererComponent
			);

			void UpdateModelRendererComponentTag(
				PREModelRendererComponent& modelRendererComponent,
				int tag
			);

			void UninitializeModelRendererComponentTag(
				PREModelRendererComponent& modelRendererComponent
			);

			void LinkCameraComponentToSkyBox(
				PRECameraComponent& cameraComponent,
				PRESkyBox& skyBox
			);
			void UnlinkCameraComponentFromSkyBox(
				PRECameraComponent& cameraComponent,
				PRESkyBox& skyBox
			);

			// ambient light
			void LinkLightToRenderTargets(
				PREAmbientLightComponent& ambientLightComponent
			);
			void UnlinkLightFromRenderTargets(
				PREAmbientLightComponent& ambientLightComponent
			);

			// point light
			void LinkLightToRenderTargets(
				PREPointLightComponent& pointLightComponent
			);
			void UnlinkLightFromRenderTargets(
				PREPointLightComponent& pointLightComponent
			);

			// spot light
			void LinkLightToRenderTargets(
				PRESpotLightComponent& spotLightComponent
			);
			void UnlinkLightFromRenderTargets(
				PRESpotLightComponent& spotLightComponent
			);

			// directional light
			void LinkLightToRenderTargets(
				PREDirectionalLightComponent& directionalLightComponent
			);
			void UnlinkLightFromRenderTargets(
				PREDirectionalLightComponent& directionalLightComponent
			);

			void LinkRenderTextureToLights(
				PRERenderTexture& renderTexture
			);
			void UnlinkRenderTextureFromLights(
				PRERenderTexture& renderTexture
			);
		};
	} // namespace Core
} // namespace PRE