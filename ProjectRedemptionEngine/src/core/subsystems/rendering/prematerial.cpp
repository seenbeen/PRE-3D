#include <core/subsystems/rendering/prematerial.h>

#include <include/modules/rendering.h>

#include <core/subsystems/rendering/prerendertexture.h>
#include <core/subsystems/rendering/preshader.h>
#include <core/subsystems/rendering/pretexture.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::RenderMaterial;

		PREMaterial::PREMaterial(RenderMaterial& material)
			:
			_material(material),
			_pShader(nullptr) {}

		void PREMaterial::SetShader(PREShader* pShader)
		{
			_pShader = pShader;
			_material.SetShaderProgram(
				_pShader != nullptr ?
					&_pShader->_shaderProgram :
					nullptr
			);
		}

		void PREMaterial::SetTextureBinding(PRETexture* pTexture, unsigned int bindUnit)
		{
			auto it = _renderTextureBindings.find(bindUnit);
			if (it != _renderTextureBindings.end())
			{
				_renderTextureBindings.erase(it);
			}
			_material.SetTextureBinding(
				pTexture != nullptr ?
					&pTexture->_texture :
					nullptr,
				bindUnit
			);
		}

		void PREMaterial::SetTextureBinding(
			PRERenderTexture& renderTexture,
			unsigned int bindUnit
		)
		{
			_renderTextureBindings[bindUnit] = &renderTexture;
		}

		PREMaterial::~PREMaterial() {}

		void PREMaterial::TempBindShadowShader()
		{
			if (_pShader != nullptr)
			{
				_material.SetShaderProgram(&_pShader->_shadowShaderProgram);
			}
		}

		void PREMaterial::TempBindCubeMapShadowShader()
		{
			if (_pShader != nullptr)
			{
				_material.SetShaderProgram(&_pShader->_cubeMapShadowShaderProgram);
			}
		}

		void PREMaterial::ResetShaderProgram()
		{
			SetShader(_pShader);
		}

		void PREMaterial::BindRenderTextureAccumulatorBindings()
		{
			for (
				auto it = _renderTextureBindings.begin();
				it != _renderTextureBindings.end();
				++it
			)
			{
				auto& renderTexture = *it->second;
				_material.SetTextureBinding(
					!renderTexture._accumulatorBufferIsA ?
						renderTexture._pBufferA :
						renderTexture._pBufferB,
					it->first
				);
			}
		}
	} // namespace Core
} // namespace PRE