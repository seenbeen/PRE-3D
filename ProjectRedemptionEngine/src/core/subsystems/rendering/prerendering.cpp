#include <core/subsystems/rendering/prerendering.h>

#include <include/modules/rendering.h>

#include <core/subsystems/rendering/prerenderingconfig.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::Renderer;

		PRERendering& PRERendering::MakePRERendering(
			const PRERenderingConfig& renderingConfig,
			PREApplicationContext& preApplicationContext
		)
		{
			auto& renderer = Renderer::MakeRenderer(
				renderingConfig.windowTitle,
				renderingConfig.windowWidth,
				renderingConfig.windowHeight
			);
			return *(new PRERendering(preApplicationContext, renderer));
		}

		PRERendering::PRERendering(
			PREApplicationContext& preApplicationContext,
			Renderer& renderer
		)
			:
			_preApplicationContext(preApplicationContext),
			_renderer(renderer) {}
		
		PRERendering::~PRERendering()
		{
			Renderer::ShutdownRenderer(_renderer);
		}

		void PRERendering::Initialize() {}

		void PRERendering::Update()
		{
			Renderer::SetActiveRenderer(_renderer);
			_renderer.Update();
		}

		void PRERendering::Shutdown() {}
	}
}