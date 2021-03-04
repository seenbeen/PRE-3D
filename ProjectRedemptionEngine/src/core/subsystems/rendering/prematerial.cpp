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
			: _material(material) {}

		void PREMaterial::SetShaderProgram(PREShader* pShader)
		{
			_material.SetShaderProgram(
				pShader != nullptr ?
					&pShader->_shaderProgram :
					nullptr
			);
		}

		void PREMaterial::SetTextureBinding(PRETexture* pTexture, unsigned int bindUnit)
		{
			_material.SetTextureBinding(
				pTexture != nullptr ?
					&pTexture->_texture :
					nullptr,
				bindUnit
			);
		}

		void PREMaterial::SetTextureBinding(
			PRERenderTexture& renderTexture,
			const CompositingAttachment& attachment,
			unsigned int bindUnit
		)
		{
			switch (attachment)
			{
			case CompositingAttachment::POSITIONS:
				_material.SetTextureBinding(
					renderTexture._compositingNode,
					RenderMaterial::CompositingAttachment::POSITIONS,
					bindUnit
				);
			case CompositingAttachment::NORMALS:
				_material.SetTextureBinding(
					renderTexture._compositingNode,
					RenderMaterial::CompositingAttachment::NORMALS,
					bindUnit
				);
			case CompositingAttachment::ALBEDO_SPECULAR:
				_material.SetTextureBinding(
					renderTexture._compositingNode,
					RenderMaterial::CompositingAttachment::ALBEDO_SPECULAR,
					bindUnit
				);
			}
		}

		PREMaterial::~PREMaterial() {}
	} // namespace Core
} // namespace PRE