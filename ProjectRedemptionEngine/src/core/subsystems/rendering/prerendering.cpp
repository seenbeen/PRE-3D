#include <core/subsystems/rendering/prerendering.h>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::Renderer;

		PRERendering::PRERendering(PREApplicationContext& preApplicationContext)
			:
			_preApplicationContext(preApplicationContext),
			_renderer(Renderer::MakeRenderer("Yes hi.", 800, 600)) {}
		
		PRERendering::~PRERendering()
		{
			Renderer::ShutdownRenderer(_renderer);
		}

		void PRERendering::Initialize()
		{
			
		}

		void PRERendering::Update()
		{
			Renderer::SetActiveRenderer(_renderer);
			_renderer.Update();
		}

		void PRERendering::Shutdown()
		{

		}
	}
}