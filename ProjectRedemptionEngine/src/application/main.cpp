#include <iostream>
#include <string>

#include <include/core.h>
#include <modules/asset/assetmanager.h>

using std::string;
using namespace PRE::Core;
using PRE::AssetModule::AssetManager;

class CubeComponent : public PREGameObjectComponent
{
protected:
    void OnStart() override
    {
        std::cout << "Start!" << std::endl;
        _transform = gameObject().GetComponent<PRETransformComponent>();
        _pMesh = &GetRendering().CreateMesh();

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

        _pMesh->SetVertices(vertices, 4);
        _pMesh->SetNormals(normals, 4);
        _pMesh->SetUvs(uvs, 4);
        _pMesh->SetTriangles(triangles, 6);

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

        _pShader = &GetRendering().CreateShader(
            vertexShaderCode,
            fragmentShadercode
        );

        _pTexture = &GetRendering().CreateTexture();
        unsigned char data[] = {
            0, 0, 0, 255,
            255, 0, 0, 255,
            0, 255, 0, 255,
            0, 0, 255, 255
        };
        _pTexture->SetData(2, 2, data);

        _pMaterial = &GetRendering().CreateMaterial();
        _pMaterial->SetShader(_pShader);
        _pMaterial->SetTextureBinding(_pTexture, 7);

        _pShader->SetInt("textureSampler", 7);

        auto& modelRendererComponent = *gameObject().GetComponent<PREModelRendererComponent>();
        modelRendererComponent.SetMesh(_pMesh);
        modelRendererComponent.SetMaterial(_pMaterial);
    }

    void OnUpdate() override
    {
         auto euler = _transform->GetEuler() + glm::vec3(60) * GetTime().GetDeltaTime();
         _transform->SetEuler(euler);
    }

    void OnDestroy() override
    {
        std::cout << "Destroy!" << std::endl;
        GetRendering().DestroyShader(*_pShader);
        GetRendering().DestroyMesh(*_pMesh);
        GetRendering().DestroyTexture(*_pTexture);
        GetRendering().DestroyMaterial(*_pMaterial);
    }

private:
    PRETransformComponent* _transform = nullptr;
    PREShader* _pShader = nullptr;
    PREMesh* _pMesh = nullptr;
    PRETexture* _pTexture = nullptr;
    PREMaterial* _pMaterial = nullptr;
};

class CameraControllerComponent : public PREGameObjectComponent
{
protected:
    void OnStart() override
    {
        auto& cameraComponent = *gameObject().GetComponent<PRECameraComponent>();
        cameraComponent.SetKind(PRECameraComponent::Kind::ORTHOGRAPHIC);
        cameraComponent.SetRenderTexture(&GetRendering().rootRenderTexture);

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
            AddPREComponent<PREModelRendererComponent>();
        }
    } cubeTemplate;

    class : public PREGameObjectTemplate
    {
    protected:
        void OnInstantiateTemplate() override
        {
            AddPREComponent<CameraControllerComponent>();
            AddPREComponent<PRECameraComponent>();
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

void AssetManagerTest()
{
    AssetManager aman(10);

    auto kFoo = "foo";
    auto pFoo = new string("foo s");

    auto kBar = "bar";
    auto pBar = new string("a bar");

    auto kBaz = "baz";
    auto pBaz = new string("zzzzzz");

    auto kQux = "qux";
    auto pQux = new string("q");

    auto Unloader = [](void* context, void* payload) {
        auto pString = static_cast<string*>(payload);
        std::cout << "Unloading \"" << *pString << "\"" << std::endl;
        delete pString;
    };

    aman.Store(
        kFoo,
        pFoo->length(),
        pFoo,
        Unloader,
        nullptr
    );

    aman.Store(
        kBar,
        pBar->length(),
        pBar,
        Unloader,
        nullptr
    );

    auto gBar = static_cast<string*>(aman.Get(kBar));
    std::cout << "got \"" << *gBar << "\"" << std::endl;
    aman.Release(kBar);

    auto gFoo = static_cast<string*>(aman.Get(kFoo));
    std::cout << "got \"" << *gFoo << "\"" << std::endl;
    aman.Release(kFoo);

    aman.Store(
        kBaz,
        pBaz->length(),
        pBaz,
        Unloader,
        nullptr
    );

    aman.Store(
        kQux,
        pQux->length(),
        pQux,
        Unloader,
        nullptr
    );

    std::cout << "---" << std::endl;
}

int main(int argc, char *argv[])
{
    //AssetManagerTest();
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