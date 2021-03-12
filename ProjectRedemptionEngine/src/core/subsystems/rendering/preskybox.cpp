#include <core/subsystems/rendering/preskybox.h>

#include <unordered_set>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::RenderModel;

		PRESkyBox::PRESkyBox(
			RenderShaderProgram& shaderProgram,
			RenderMesh& mesh,
			RenderTexture& texture,
			RenderMaterial& material,
			RenderModel& model
		)
			:
			_shaderProgram(shaderProgram),
			_mesh(mesh),
			_texture(texture),
			_material(material),
			_model(model) {}

		PRESkyBox::~PRESkyBox() {}
	} // namespace Core
} // namespace PRE