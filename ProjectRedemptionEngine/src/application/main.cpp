#include <iostream>
#include <string>

#include <include/core.h>

using std::string;
using namespace PRE::Core;

class CubeComponent : public PREGameObjectComponent
{
protected:
    void OnStart() override
    {
        std::cout << "Start!" << std::endl;
        _transform = gameObject().GetComponent<PRETransformComponent>();
    }

    void OnUpdate() override
    {
         auto euler = _transform->GetEuler() + glm::vec3(60) * GetTime().GetDeltaTime();
         _transform->SetEuler(euler);
    }

    void OnDestroy() override
    {
        std::cout << "Destroy!" << std::endl;
    }

private:
    PRETransformComponent* _transform = nullptr;
};

class CameraControllerComponent : public PREGameObjectComponent
{
protected:
    void OnStart() override
    {
        gameObject().GetComponent<PRETransformComponent>()->SetPosition(
            glm::vec3(0, 0, 2)
        );
    }

    void OnUpdate() override
    {
        if (GetInput().ApplicationHasQuit())
        {
            std::cout << "Application Quitting..." << std::endl;
            Quit();
        }
        if (GetInput().MouseButtonLeftPressed())
        {
            int x, y;
            GetInput().MousePosition(x, y);
            std::cout << "PRESSED AT (" << x << ", " << y << ")" << std::endl;
        }
        if (GetInput().MouseButtonLeftReleased())
        {
            int x, y;
            GetInput().MousePosition(x, y);
            std::cout << "RELEASED AT (" << x << ", " << y << ")" << std::endl;
        }
        if (GetInput().KeyPressed(PREKeyCode::SPACE))
        {
            std::cout << "~" << 1 / GetTime().GetDeltaTime() << " FPS." << std::endl;
        }
    }
};

void OnInitialize(PREApplicationContext& applicationContext)
{
    std::cout << "ON INITIALIZE" << std::endl;
    // applicationContext.time.SetFrameLimit(60);
    class : public PREGameObjectTemplate
    {
    protected:
        void OnInstantiateTemplate() override
        {
            AddPREComponent<CubeComponent>();
            auto& modelRendererComponent = *AddPREComponent<PREModelRendererComponent>();
            auto& mesh = GetRendering().CreateMesh();

            glm::vec3 vertices[] = {
                glm::vec3(-1, -1, 0),
                glm::vec3(1, -1, 0),
                glm::vec3(1, 1, 0),
                glm::vec3(-1, 1, 0)
            };
            glm::vec3 normals[] = {
                glm::vec3(0, 1, 0),
                glm::vec3(-1, 0, 0),
                glm::vec3(0, -1, 0),
                glm::vec3(1, 0, 0)
            };
            glm::vec2 uvs[] = {
                glm::vec2(0, 0),
                glm::vec2(0, 1),
                glm::vec2(1, 1),
                glm::vec2(1, 0)
            };

            unsigned int triangles[] = { 0, 1, 2, 0, 2, 3 };

            mesh.SetVertices(vertices, 4);
            mesh.SetNormals(normals, 4);
            mesh.SetUvs(uvs, 4);
            mesh.SetTriangles(triangles, 6);

            string vertexShaderCode(
                "#version 330 core\n"
                "layout (location = 0) in vec3 iPos;\n"
                "layout (location = 1) in vec3 iNorm;\n"
                "layout (location = 2) in vec2 iUV;\n"
                "\n"
                "uniform mat4 PRE_MVP;\n"
                "\n"
                "out vec2 TexCoord;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    gl_Position = PRE_MVP * vec4(iPos, 1.0);\n"
                "    TexCoord = iUV;\n"
                "}\n"
            );

            string fragmentShadercode(
                "#version 330 core\n"
                "out vec4 FragColor;\n"
                "\n"
                "in vec2 TexCoord;\n"
                "\n"
                "uniform sampler2D textureSampler;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    FragColor = texture(textureSampler, TexCoord);\n"
                "}\n"
            );

            auto& shader = GetRendering().CreateShader(
                vertexShaderCode,
                fragmentShadercode
            );

            auto& texture = GetRendering().CreateTexture();
            unsigned char data[] = {
                0, 0, 0, 255,
                255, 0, 0, 255,
                0, 255, 0, 255,
                0, 0, 255, 255
            };
            texture.SetData(2, 2, data);

            auto& material = GetRendering().CreateMaterial();
            material.SetShader(&shader);
            material.SetTextureBinding(&texture, 7);

            shader.SetInt("textureSampler", 7);

            modelRendererComponent.SetMesh(&mesh);
            modelRendererComponent.SetMaterial(&material);
        }
    } cubeTemplate;

    class : public PREGameObjectTemplate
    {
    protected:
        void OnInstantiateTemplate() override
        {
            AddPREComponent<CameraControllerComponent>();
            auto& cameraComponent = *AddPREComponent<PRECameraComponent>();
            cameraComponent.SetKind(PRECameraComponent::Kind::ORTHOGRAPHIC);
            cameraComponent.SetRenderTexture(&GetRendering().rootRenderTexture);
        }
    } cameraTemplate;

    auto& cube = applicationContext.world.Instantiate(cubeTemplate);
    auto& modelRendererComponent = *cube.GetComponent<PREModelRendererComponent>();

    auto& camera = applicationContext.world.Instantiate(cameraTemplate);
    auto pCameraComponent = camera.GetComponent<PRECameraComponent>();

    modelRendererComponent.SetCameraComponent(pCameraComponent);
}

void OnShutdown(PREApplicationContext& applicationContext)
{
    std::cout << "ON SHUTDOWN" << std::endl;
}

int main(int argc, char *argv[])
{
    PREApplication::Run(
        PREApplicationConfig(
            PREInputConfig(),
            PRERenderingConfig(
                "Yes hi.",
                800,
                600
            ),
            PRETimeConfig(),
            PREWorldConfig(),
            OnInitialize,
            OnShutdown
        )
    );
    return 0;
}