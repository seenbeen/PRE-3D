#include <modules/rendering/model/rendermaterial.h>

#include <glad/glad.h>

#include <modules/rendering/camera/camera.h>

#include <modules/rendering/shader/rendershaderprogram.h>
#include <modules/rendering/model/rendertexture.h>

namespace PRE
{
	namespace RenderingModule
	{
		RenderMaterial::Impl& RenderMaterial::Impl::MakeImpl(RenderShaderProgram& shaderProgram)
		{
			return *(new Impl(shaderProgram));
		}

		RenderMaterial::Impl::Impl(RenderShaderProgram& shaderProgram)
			:
			shaderProgram(&shaderProgram) {}

		RenderMaterial::Impl::~Impl() {}

		RenderMaterial::RenderMaterial(RenderShaderProgram& shaderProgram)
			:
			_impl(Impl::MakeImpl(shaderProgram)) {}

		RenderMaterial::~RenderMaterial()
		{
			delete &_impl;
		}

		void RenderMaterial::SetShaderProgram(RenderShaderProgram& shaderProgram)
		{
			_impl.shaderProgram = &shaderProgram;
		}

		void RenderMaterial::SetTextureBinding(RenderTexture* pRenderTexture, GLenum bindUnit)
		{
			if (pRenderTexture == nullptr)
			{
				auto it = _impl.textureBindings.find(bindUnit);
				if (it != _impl.textureBindings.end())
				{
					_impl.textureBindings.erase(it);
				}
			}
			else
			{
				_impl.textureBindings[bindUnit] = pRenderTexture;
			}
		}

		void RenderMaterial::Bind(const Camera& camera)
		{
			for (auto it = _impl.textureBindings.begin(); it != _impl.textureBindings.end(); ++it)
			{
				RenderTexture::SetActive(it->first);
				it->second->Bind();
			}
			_impl.shaderProgram->Bind();
			_impl.shaderProgram->SetMat4("PRE_MVP", camera.GetViewProjectionMatrix());
		}
	} // namespace RenderingModule
} // namespace PRE