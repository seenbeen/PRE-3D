#include <core/subsystems/rendering/prerendering.h>

#include <include/modules/rendering.h>

#include <core/components/precameracomponent.h>
#include <core/components/premodelrenderercomponent.h>
#include <core/components/pretransformcomponent.h>

#include <core/subsystems/rendering/prerenderingconfig.h>

#include <core/subsystems/rendering/prerendertexture.h>
#include <core/subsystems/rendering/preshader.h>
#include <core/subsystems/rendering/premesh.h>
#include <core/subsystems/rendering/pretexture.h>
#include <core/subsystems/rendering/prematerial.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::Renderer;
		using PRE::RenderingModule::RenderCamera;
		using PRE::RenderingModule::RenderModel;

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
			applicationContext(applicationContext),
			renderer(renderer) {}

		PRERendering::Impl::~Impl()
		{
			Renderer::ShutdownRenderer(renderer);
		}

		PRERenderTexture& PRERendering::CreateRenderTexture()
		{

		}

		void PRERendering::DestroyRenderTexture(PRERenderTexture& renderTexture)
		{

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
		}

		PRETexture& PRERendering::CreateTexture()
		{
			return *(new PRETexture(_impl.renderer.AllocateTexture()));
		}

		void PRERendering::DestroyTexture(PRETexture& texture)
		{
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

		PREMesh& PRERendering::CreateMesh()
		{
			return *(new PREMesh(_impl.renderer.AllocateMesh()));
		}

		void PRERendering::DestroyMesh(PREMesh& mesh)
		{
			_impl.renderer.DeallocateMesh(mesh._mesh);
			delete &mesh;
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
			_impl(Impl::MakeImpl(applicationContext, renderer)) {}
		
		PRERendering::~PRERendering()
		{
			delete &_impl;
		}

		void PRERendering::Initialize() {}

		void PRERendering::Update()
		{
			Renderer::SetActiveRenderer(_impl.renderer);
			_impl.renderer.Update();
		}

		void PRERendering::Shutdown() {}

		/*
		void PRERendering::BindCompositingPair(
			PRECameraComponent& camera,
			PRERenderTexture* pRenderTexture
		)
		{
			if (pRenderTexture == nullptr)
			{
				_impl.renderer.UnbindCompositingPair(
					*camera._pCamera,
					_impl.renderer.rootCompositingNode
				);
			}
			else
			{
				_impl.renderer.BindCompositingPair(
					*camera._pCamera,
					pRenderTexture->_compositingNode
				);
			}
		}
		*/

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

				// GetRendering().BindCompositingPair(*this, _pRenderTexture);
			}
		}

		void PRERendering::DeallocateCamera(PRECameraComponent& cameraComponent)
		{
			_impl.renderer.DeallocateCamera(*cameraComponent._pCamera);
		}

		void PRERendering::AllocateModel(PREModelRendererComponent& modelRendererComponent)
		{
			auto& model = _impl.renderer.AllocateModel();
			_impl.renderer.AddModelToTagGroup(
				*modelRendererComponent._pModel,
				modelRendererComponent._tagGroup
			);
			modelRendererComponent._pModel = &model;
		}

		void PRERendering::UpdateModel(
			PREModelRendererComponent& modelRendererComponent
		)
		{
			modelRendererComponent._pModel->modelMatrix = modelRendererComponent._pTransformComponent->GetMatrix();
			if (modelRendererComponent._hasChanged)
			{
				_impl.renderer.SetModelMesh(
					*modelRendererComponent._pModel,
					modelRendererComponent._pMesh != nullptr ?
					&modelRendererComponent._pMesh->_mesh :
					nullptr
				);
				_impl.renderer.SetModelMaterial(
					*modelRendererComponent._pModel,
					modelRendererComponent._pMaterial != nullptr ?
					&modelRendererComponent._pMaterial->_material :
					nullptr
				);
				_impl.renderer.RemoveModelFromTagGroup(
					*modelRendererComponent._pModel,
					modelRendererComponent._oldTagGroup
				);
				_impl.renderer.AddModelToTagGroup(
					*modelRendererComponent._pModel,
					modelRendererComponent._tagGroup
				);
				modelRendererComponent._oldTagGroup = modelRendererComponent._tagGroup;
			}
		}

		void PRERendering::DeallocateModel(PREModelRendererComponent& modelRendererComponent)
		{
			_impl.renderer.RemoveModelFromTagGroup(
				*modelRendererComponent._pModel,
				modelRendererComponent._tagGroup
			);
			_impl.renderer.DeallocateModel(*modelRendererComponent._pModel);
		}
	}
}