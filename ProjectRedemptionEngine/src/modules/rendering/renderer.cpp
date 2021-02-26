#include <modules/rendering/renderer.h>

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <SDL.h>
#undef main

#include <glad/glad.h>

#include <modules/rendering/shader/rendervertexshader.h>
#include <modules/rendering/shader/renderfragmentshader.h>
#include <modules/rendering/shader/rendershaderprogram.h>

#include <modules/rendering/compositing/compositingnode.h>
#include <modules/rendering/compositing/compositingtarget.h>

#include <modules/rendering/camera/camera.h>
#include <modules/rendering/rendermodel.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::list;
		using std::string;
		using std::unordered_map;
		using std::unordered_set;

		Renderer::Renderer()
		{

		}

		Renderer::~Renderer()
		{

		}

		void Renderer::Initialize()
		{
/*
	#ifdef __EMSCRIPTEN__
		emscripten_set_canvas_element_size("#canvas", SCREEN_WIDTH, SCREEN_HEIGHT);
		EmscriptenWebGLContextAttributes attr;
		emscripten_webgl_init_context_attributes(&attr);
		attr.alpha = attr.depth = attr.stencil = attr.antialias = attr.preserveDrawingBuffer = attr.failIfMajorPerformanceCaveat = 0;
		attr.enableExtensionsByDefault = 1;
		attr.premultipliedAlpha = 0;
		attr.majorVersion = 1;
		attr.minorVersion = 0;
		EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);
		emscripten_webgl_make_context_current(ctx);

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			printf("GLEW init failed: %s!\n", glewGetErrorString(err));
			success = false;
		}

		//Initialize OpenGL
		if (!initGL(userData))
		{
			printf("Unable to initialize OpenGL!\n");
			success = false;
		}
	#else

		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Use OpenGL 2.1
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

			//Create window
			gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
			if (gWindow == NULL)
			{
				printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Create context
				gContext = SDL_GL_CreateContext(gWindow);
				if (gContext == NULL)
				{
					printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
					success = false;
				}
				else
				{
					//Use Vsync
					if (SDL_GL_SetSwapInterval(1) < 0)
					{
						printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
					}

					GLenum err = glewInit();
					if (GLEW_OK != err)
					{
						printf("GLEW init failed: %s!\n", glewGetErrorString(err));
						success = false;
					}

					//Initialize OpenGL
					if (!initGL(userData))
					{
						printf("Unable to initialize OpenGL!\n");
						success = false;
					}
				}
			}
		}
	#endif
*/
		}

		void Renderer::Update()
		{

		}

		void Renderer::Shutdown()
		{

		}

		const RenderVertexShader& Renderer::AllocateVertexShader(const string& shaderSource)
		{
			auto vertexShader = new RenderVertexShader(shaderSource);
			_vertexShaders.insert(vertexShader);
			return *vertexShader;
		}

		void Renderer::DeallocateVertexShader(const RenderVertexShader& vertexShader)
		{
			auto pVertexShader = &vertexShader;
			auto it = _vertexShaders.find(pVertexShader);
			if (it == _vertexShaders.end())
			{
				throw "Cannot deallocate unknown RenderVertexShader";
			}
			_vertexShaders.erase(it);
			delete pVertexShader;
		}

		const RenderFragmentShader& Renderer::AllocateFragmentShader(const string& shaderSource)
		{
			auto fragmentShader = new RenderFragmentShader(shaderSource);
			_fragmentShaders.insert(fragmentShader);
			return *fragmentShader;
		}

		void Renderer::DeallocateFragmentShader(const RenderFragmentShader& fragmentShader)
		{
			auto pFragmentShader = &fragmentShader;
			auto it = _fragmentShaders.find(pFragmentShader);
			if (it == _fragmentShaders.end())
			{
				throw "Cannot deallocate unknown RenderFragmentShader";
			}
			_fragmentShaders.erase(it);
			delete pFragmentShader;
		}

		const RenderShaderProgram& Renderer::AllocateShaderProgram(
			const RenderVertexShader& vertexShader,
			const RenderFragmentShader& fragmentShader
		)
		{
			auto shaderProgram = new RenderShaderProgram(vertexShader, fragmentShader);
			_shaderPrograms.insert(shaderProgram);
			return *shaderProgram;
		}

		void Renderer::DeallocateShaderProgram(const RenderShaderProgram& shaderProgram)
		{
			auto pShaderProgram = &shaderProgram;
			auto it = _shaderPrograms.find(pShaderProgram);
			if (it == _shaderPrograms.end())
			{
				throw "Cannot deallocate unknown RenderShaderProgram";
			}
			_shaderPrograms.erase(it);
			delete pShaderProgram;
		}

		RenderModel& Renderer::AllocateRenderModel()
		{

		}

		void Renderer::DeallocateRenderModel(RenderModel& renderModel)
		{

		}

		CompositingNode& Renderer::AllocateCompositingNode()
		{

		}

		void Renderer::AttachCompositingNode(CompositingNode& compositingNode, CompositingNode& dependency)
		{

		}

		void Renderer::DetachCompositingNodeDependency(CompositingNode& compositingNode, CompositingNode& dependency)
		{

		}

		void Renderer::DeallocateCompositingNode(CompositingNode& compositingNode)
		{

		}

		CompositingTarget& Renderer::AllocateCompositingTarget()
		{

		}

		void Renderer::DeallocateCompositingTarget(CompositingTarget& compositingTarget)
		{

		}

		void Renderer::Render(Camera& camera, CompositingTarget& compositingTarget)
		{
			// TODO: spatial query optimization
			// foreach model, render using cam to compositing target
		}
	} // namespace RenderingModule
} // namespace PRE