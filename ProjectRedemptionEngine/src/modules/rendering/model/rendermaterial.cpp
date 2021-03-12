#include <modules/rendering/model/rendermaterial.h>

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <modules/rendering/compositing/rendercompositingtarget.h>
#include <modules/rendering/compositing/rendercompositingnode.h>
#include <modules/rendering/shader/rendershaderprogram.h>

#include <modules/rendering/texture/rendertexture.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::vector;

		RenderMaterial::Impl& RenderMaterial::Impl::MakeImpl()
		{
			return *(new Impl());
		}

		RenderMaterial::Impl::Impl()
			:
			pShaderProgram(nullptr) {}

		RenderMaterial::Impl::~Impl() {}

		void RenderMaterial::SetShaderProgram(RenderShaderProgram* pShaderProgram)
		{
			_impl.pShaderProgram = pShaderProgram;
		}

		void RenderMaterial::SetTextureBinding(RenderTexture* pRenderTexture, GLenum bindUnit)
		{
			bindUnit = GL_TEXTURE0 + bindUnit;
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

		void RenderMaterial::SetTextureBinding(
			RenderCompositingNode& compositingNode,
			GLenum bindUnit
		)
		{
			SetTextureBinding(
				&compositingNode.GetTarget(),
				bindUnit
			);
		}

		RenderMaterial::RenderMaterial()
			:
			_impl(Impl::MakeImpl()) {}

		RenderMaterial::~RenderMaterial()
		{
			delete &_impl;
		}

		void RenderMaterial::Bind(
			const glm::mat4& mvp,
			const vector<glm::mat4>* pBoneTransforms
		)
		{
			if (_impl.pShaderProgram == nullptr)
			{
				return;
			}
			for (auto it = _impl.textureBindings.begin(); it != _impl.textureBindings.end(); ++it)
			{
				RenderTexture::SetActive(it->first);
				it->second->Bind();
			}
			_impl.pShaderProgram->Bind();
			_impl.pShaderProgram->SetMat4("PRE_MVP", mvp);
			if (pBoneTransforms != nullptr)
			{
				auto& boneTransforms = *pBoneTransforms;
				_impl.pShaderProgram->SetMat4(
					"PRE_BONETRANSFORMS",
					&boneTransforms[0],
					(unsigned int)boneTransforms.size()
				);
			}
		}
	} // namespace RenderingModule
} // namespace PRE