#include <core/subsystems/rendering/prerendering.h>

#include <vector>

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
#include <core/subsystems/rendering/preanimation.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::Renderer;
		using PRE::RenderingModule::RenderCamera;
		using PRE::RenderingModule::RenderModel;

		using PRE::AnimationModule::Animation;

		using std::vector;

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
			_impl.applicationContext.assetManager.TryFreeShader(shader);
			_impl.renderer.DeallocateShaderProgram(shader._shaderProgram);
			delete &shader;
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
			_impl.applicationContext.assetManager.TryFreeTexture(texture);
			_impl.renderer.DeallocateTexture(texture._texture);
			delete &texture;
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
			_impl.applicationContext.assetManager.TryFreeMesh(mesh);
			_impl.renderer.DeallocateMesh(mesh._mesh);
			delete &mesh;
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
			_impl.applicationContext.assetManager.TryFreeSkeleton(skeleton);
			_impl.renderer.DeallocateSkeleton(skeleton._skeleton);
			delete &skeleton;
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
			delete &animation;
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

		void PRERendering::AllocateCamera(PRECameraComponent& cameraComponent)
		{
			auto& camera = _impl.renderer.AllocateCamera(
				cameraComponent._kind == PRECameraComponent::Kind::ORTHOGRAPHIC ?
					Renderer::CameraKind::ORTHOGRAPHIC :
					Renderer::CameraKind::PERSPECTIVE,
				cameraComponent._size,
				cameraComponent._aspectRatio,
				cameraComponent._nearClippingPlane,
				cameraComponent._farClippingPlane
			);
			cameraComponent._pCamera = &camera;
		}

		void PRERendering::UpdateCamera(PRECameraComponent& cameraComponent)
		{
			cameraComponent._pCamera->SetViewMatrix(
				cameraComponent._pTransformComponent->GetInverseMatrix()
			);

			if (cameraComponent._hasChanged)
			{
				cameraComponent._hasChanged = false;
				cameraComponent._pCamera->SetKind(
					cameraComponent._kind == PRECameraComponent::Kind::ORTHOGRAPHIC ?
					RenderCamera::Kind::ORTHOGRAPHIC :
					RenderCamera::Kind::PERSPECTIVE
				);
				cameraComponent._pCamera->SetSize(cameraComponent._size);
				cameraComponent._pCamera->SetAspectRatio(cameraComponent._aspectRatio);
				cameraComponent._pCamera->SetNearClippingPlane(cameraComponent._nearClippingPlane);
				cameraComponent._pCamera->SetFarClippingPlane(cameraComponent._farClippingPlane);
			}

			auto pPreviousRenderTexture = cameraComponent._pPreviousRenderTexture;
			auto pRenderTexture = cameraComponent._pRenderTexture;
			auto& associatedModelComponents = cameraComponent._associatedModelComponents;

			if (pPreviousRenderTexture != pRenderTexture)
			{
				if (pPreviousRenderTexture != nullptr)
				{
					for (auto it = associatedModelComponents.begin(); it != associatedModelComponents.end(); ++it)
					{
						auto& modelRendererComponent = **it;
						_impl.renderer.RemoveModelFromTagGroup(
							*modelRendererComponent._pModel,
							pPreviousRenderTexture->_tagGroup
						);
					}

					_impl.renderer.UnbindCompositingPair(
						*cameraComponent._pCamera,
						pPreviousRenderTexture->_compositingNode
					);
				}

				if (pRenderTexture != nullptr)
				{
					_impl.renderer.BindCompositingPair(
						*cameraComponent._pCamera,
						pRenderTexture->_compositingNode
					);

					for (auto it = associatedModelComponents.begin(); it != associatedModelComponents.end(); ++it)
					{
						auto& modelRendererComponent = **it;
						_impl.renderer.AddModelToTagGroup(
							*modelRendererComponent._pModel,
							pRenderTexture->_tagGroup
						);
					}
				}

				cameraComponent._pPreviousRenderTexture = pRenderTexture;
			}
		}

		void PRERendering::DeallocateCamera(PRECameraComponent& cameraComponent)
		{
			auto pRenderTexture = cameraComponent._pRenderTexture;
			auto& associatedModelComponents = cameraComponent._associatedModelComponents;

			if (pRenderTexture != nullptr)
			{
				for (auto it = associatedModelComponents.begin(); it != associatedModelComponents.end(); ++it)
				{
					auto& modelRendererComponent = **it;
					_impl.renderer.RemoveModelFromTagGroup(
						*modelRendererComponent._pModel,
						pRenderTexture->_tagGroup
					);
					modelRendererComponent._pCameraComponent = nullptr;
				}

				_impl.renderer.UnbindCompositingPair(
					*cameraComponent._pCamera,
					pRenderTexture->_compositingNode
				);

				// not necessary to re-assign pRenderTexture values
				// ie: cameraComponent._pPreviousRenderTexture = cameraComponent._pRenderTexture = nullptr;
			}

			_impl.renderer.DeallocateCamera(*cameraComponent._pCamera);
		}

		void PRERendering::AllocateModel(PREModelRendererComponent& modelRendererComponent)
		{
			auto& model = _impl.renderer.AllocateModel();
			modelRendererComponent._pModel = &model;
		}

		void PRERendering::UpdateModel(
			PREModelRendererComponent& modelRendererComponent
		)
		{
			modelRendererComponent._pModel->modelMatrix = modelRendererComponent._pTransformComponent->GetMatrix();

			if (modelRendererComponent._hasChanged)
			{
				modelRendererComponent._hasChanged = false;

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

			auto pPreviousCameraComponent = modelRendererComponent._pPreviousCameraComponent;
			auto pCameraComponent = modelRendererComponent._pCameraComponent;

			if (pPreviousCameraComponent != pCameraComponent)
			{
				if (pPreviousCameraComponent != nullptr)
				{
					if (pPreviousCameraComponent->_pPreviousRenderTexture != nullptr)
					{
						_impl.renderer.RemoveModelFromTagGroup(
							*modelRendererComponent._pModel,
							pPreviousCameraComponent->_pPreviousRenderTexture->_tagGroup
						);
					}
					pPreviousCameraComponent->_associatedModelComponents.erase(
						&modelRendererComponent
					);
				}

				if (pCameraComponent != nullptr)
				{
					pCameraComponent->_associatedModelComponents.insert(
						&modelRendererComponent
					);
					if (pCameraComponent->_pPreviousRenderTexture != nullptr)
					{
						_impl.renderer.AddModelToTagGroup(
							*modelRendererComponent._pModel,
							pCameraComponent->_pPreviousRenderTexture->_tagGroup
						);
					}
				}

				modelRendererComponent._pPreviousCameraComponent = pCameraComponent;
			}
		}

		void PRERendering::DeallocateModel(PREModelRendererComponent& modelRendererComponent)
		{
			auto pCameraComponent = modelRendererComponent._pCameraComponent;

			if (pCameraComponent != nullptr)
			{
				if (pCameraComponent->_pPreviousRenderTexture != nullptr)
				{
					_impl.renderer.RemoveModelFromTagGroup(
						*modelRendererComponent._pModel,
						pCameraComponent->_pPreviousRenderTexture->_tagGroup
					);
				}
				pCameraComponent->_associatedModelComponents.erase(
					&modelRendererComponent
				);

				// not necessary to re-assign pCamera tracker values
				// ie: modelRendererComponent._pPreviousCamera = modelRendererComponent._pCamera;
			}

			_impl.renderer.DeallocateModel(*modelRendererComponent._pModel);
		}
	}
}