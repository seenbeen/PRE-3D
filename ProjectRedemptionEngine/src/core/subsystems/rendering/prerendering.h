#pragma once
#include <string>

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

		class PRERenderTexture;
		class PREShader;
		class PREMesh;
		class PRESkeleton;
		class PREBoneConfig;
		class PRETexture;
		class PREMaterial;

		class PREAnimation;
		class PREAnimationConfig;
		class PREAnimator;
		class PREAnimatorConfig;

		class PRESkyBox;

		using std::string;
		using PRE::RenderingModule::Renderer;
		using PRE::RenderingModule::RenderCompositingNode;
		using PRE::RenderingModule::RenderCamera;
		using PRE::RenderingModule::RenderModel;

		class PRERendering
		{
			PRERendering& operator=(const PRERendering&) = delete;
			PRERendering(const PRERendering&) = delete;

			friend class PREApplicationContext;
			friend class PREModelRendererComponent;
			friend class PRECameraComponent;
			friend class PRERenderTexture;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class PRERendering;

			private:
				static Impl& MakeImpl(PREApplicationContext& applicationContext, Renderer& renderer);

				PREApplicationContext& applicationContext;
				Renderer& renderer;

				Impl(PREApplicationContext& applicationContext, Renderer& renderer);
				~Impl();
			};

		public:
			PRERenderTexture& GetScreenRenderTexture();

			PRERenderTexture& CreateRenderTexture(unsigned int width, unsigned int height);
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
			enum class CameraKind { ORTHOGRAPHIC, PERSPECTIVE };

			static PRERendering& MakePRERendering(
				const PRERenderingConfig& renderingConfig,
				PREApplicationContext& applicationContext
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

			void ComposeRenderComposition(
				RenderCompositingNode::RenderComposition& composition,
				PRECameraComponent& cameraComponent
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

			void LinkModelRendererComponentToCameraComponent(
				PREModelRendererComponent& modelRendererComponent,
				PRECameraComponent& cameraComponent
			);
			void UnlinkModelRendererComponentFromCameraComponent(
				PREModelRendererComponent& modelRendererComponent,
				PRECameraComponent& cameraComponent
			);

			void LinkCameraComponentToSkyBox(
				PRECameraComponent& cameraComponent,
				PRESkyBox& skyBox
			);
			void UnlinkCameraComponentFromSkyBox(
				PRECameraComponent& cameraComponent,
				PRESkyBox& skyBox
			);
		};
	} // namespace Core
} // namespace PRE