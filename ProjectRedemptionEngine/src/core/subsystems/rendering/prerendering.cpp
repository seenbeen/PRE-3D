#include <core/subsystems/rendering/prerendering.h>

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <include/modules/rendering.h>
#include <include/modules/animation.h>

#include <core/preapplicationcontext.h>

#include <core/components/precameracomponent.h>
#include <core/components/premodelrenderercomponent.h>
#include <core/components/pretransformcomponent.h>

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

#ifdef __PRE_DEBUG__
#include <assert.h>
#endif

namespace PRE
{
	namespace Core
	{
		using std::string;
		using std::vector;

		using PRE::RenderingModule::Renderer;
		using PRE::RenderingModule::RenderCamera;
		using PRE::RenderingModule::RenderModel;

		using PRE::AnimationModule::Animation;

		PRERendering::Impl& PRERendering::Impl::MakeImpl(
			PREApplicationContext& applicationContext,
			Renderer& renderer
		)
		{
			return *(new Impl(applicationContext, renderer));
		}

		PRERendering::Impl::Impl(
			PREApplicationContext& applicationContext,
			Renderer& renderer
		)
			:
			tagGroupCounter(0),
			applicationContext(applicationContext),
			renderer(renderer) {}

		PRERendering::Impl::~Impl()
		{
			Renderer::ShutdownRenderer(renderer);
		}

		PRERenderTexture& PRERendering::CreateRenderTexture(unsigned int width, unsigned int height)
		{
			_impl.renderer.DeclareTagGroup(++_impl.tagGroupCounter);
			return *(
				new PRERenderTexture(
					_impl.tagGroupCounter,
					_impl.renderer.AllocateCompositingNode(
						_impl.tagGroupCounter,
						width,
						height
					)
				)
			);
		}

		void PRERendering::DestroyRenderTexture(PRERenderTexture& renderTexture)
		{
			auto tagGroup = renderTexture._tagGroup;
			_impl.renderer.DeallocateCompositingNode(renderTexture._compositingNode);
			delete &renderTexture;
			_impl.renderer.RevokeTagGroup(tagGroup);
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
			"uniform mat4 PRE_MODEL_VIEW_MATRIX;\n"
			"uniform mat4 PRE_PROJECTION_MATRIX;\n"
			"\n"
			"out vec3 TexCoord;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    vec4 pos = PRE_PROJECTION_MATRIX * mat4(mat3(PRE_MODEL_VIEW_MATRIX)) * vec4(iPos, 1.0);\n"
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
			rootRenderTexture(
				*(
					new PRERenderTexture(
						Renderer::ROOT_TAG_GROUP,
						renderer.rootCompositingNode
					)
				)
			),
			_impl(Impl::MakeImpl(applicationContext, renderer)) {}
		
		PRERendering::~PRERendering()
		{
			delete &rootRenderTexture;
			delete &_impl;
		}

		void PRERendering::Initialize() {}

		void PRERendering::Update()
		{
			Renderer::SetActiveRenderer(_impl.renderer);
			_impl.renderer.Update();
		}

		void PRERendering::Shutdown() {}

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

		// Note: PRERenderTextures aren't associated with cameras because
		// render textures can't be destroyed manually (the engine does
		// deallocating at end of life, at which point all associations
		// should already be unlinked)

		void PRERendering::LinkCameraComponentToRenderTexture(
			PRECameraComponent& cameraComponent,
			PRERenderTexture& renderTexture
		)
		{
#ifdef __PRE_DEBUG__
			assert(cameraComponent._pRenderTexture == nullptr);
#endif

			for (
				auto it = cameraComponent._associatedModelComponents.begin();
				it != cameraComponent._associatedModelComponents.end();
				++it
				)
			{
				auto& modelRendererComponent = **it;
				_impl.renderer.AddModelToTagGroup(
					*modelRendererComponent._pModel,
					renderTexture._tagGroup
				);
			}

			if (cameraComponent._pSkyBox != nullptr)
			{
				_impl.renderer.AddModelToTagGroup(
					cameraComponent._pSkyBox->_model,
					renderTexture._tagGroup
				);
			}

			_impl.renderer.BindCompositingPair(
				*cameraComponent._pCamera,
				renderTexture._compositingNode
			);

			cameraComponent._pRenderTexture = &renderTexture;
		}

		void PRERendering::UnlinkCameraComponentFromRenderTexture(
			PRECameraComponent& cameraComponent,
			PRERenderTexture& renderTexture
		)
		{
#ifdef __PRE_DEBUG__
			assert(cameraComponent._pRenderTexture == &renderTexture);
#endif

			for (
				auto it = cameraComponent._associatedModelComponents.begin();
				it != cameraComponent._associatedModelComponents.end();
				++it
				)
			{
				auto& modelRendererComponent = **it;
				_impl.renderer.RemoveModelFromTagGroup(
					*modelRendererComponent._pModel,
					renderTexture._tagGroup
				);
			}

			if (cameraComponent._pSkyBox != nullptr)
			{
				_impl.renderer.RemoveModelFromTagGroup(
					cameraComponent._pSkyBox->_model,
					renderTexture._tagGroup
				);
			}

			_impl.renderer.UnbindCompositingPair(
				*cameraComponent._pCamera,
				renderTexture._compositingNode
			);

			cameraComponent._pRenderTexture = nullptr;
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

			if (cameraComponent._pRenderTexture != nullptr)
			{
				_impl.renderer.AddModelToTagGroup(
					*modelRendererComponent._pModel,
					cameraComponent._pRenderTexture->_tagGroup
				);
			}

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

			if (cameraComponent._pRenderTexture != nullptr)
			{
				_impl.renderer.RemoveModelFromTagGroup(
					*modelRendererComponent._pModel,
					cameraComponent._pRenderTexture->_tagGroup
				);
			}

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

			if (cameraComponent._pRenderTexture != nullptr)
			{
				_impl.renderer.AddModelToTagGroup(
					skyBox._model,
					cameraComponent._pRenderTexture->_tagGroup
				);
			}

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

			if (cameraComponent._pRenderTexture != nullptr)
			{
				_impl.renderer.RemoveModelFromTagGroup(
					skyBox._model,
					cameraComponent._pRenderTexture->_tagGroup
				);
			}

			cameraComponent._pSkyBox = nullptr;
			skyBox._associatedCameraComponents.erase(&cameraComponent);
		}
	}
}