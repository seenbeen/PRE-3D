#include <iostream>
#include <string>

#include <include/core.h>

using std::string;
using namespace PRE::Core;

class SimpleModelComponent : public PREGameObjectComponent
{
public:
    string vertexShaderPath;
    string fragmentShaderPath;
    string meshPath;
    string albedoPath;

protected:
    void OnStart() override
    {
        std::cout << "Start!" << std::endl;
        _transform = gameObject().GetComponent<PRETransformComponent>();

        _pShader = &GetAssetManager().LoadShader(
            GetAssetManager().rootAssetPath + vertexShaderPath,
            GetAssetManager().rootAssetPath + fragmentShaderPath
        );

        _pMesh = &GetAssetManager().LoadMesh(
            GetAssetManager().rootAssetPath + meshPath
        );

        _pTexture = &GetAssetManager().LoadTexture(
            GetAssetManager().rootAssetPath + albedoPath
        );

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
         auto euler = _transform->GetLocalEuler() + glm::vec3(0, 60, 0) * GetTime().GetDeltaTime();
         _transform->SetLocalEuler(euler);
    }

    void OnDestroy() override
    {
        std::cout << "Destroy!" << std::endl;
        GetAssetManager().FreeShader(
            GetAssetManager().rootAssetPath + vertexShaderPath,
            GetAssetManager().rootAssetPath + fragmentShaderPath
        );
        GetAssetManager().FreeMesh(
            GetAssetManager().rootAssetPath + meshPath
        );
        GetAssetManager().FreeTexture(
            GetAssetManager().rootAssetPath + albedoPath
        );
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
        cameraComponent.SetKind(PRECameraComponent::Kind::PERSPECTIVE);
        cameraComponent.SetRenderTexture(&GetRendering().rootRenderTexture);
        cameraComponent.SetFarClippingPlane(1000);
        cameraComponent.SetSize(90);
        gameObject().GetComponent<PRETransformComponent>()->SetPosition(
            glm::vec3(0, 0, 10)
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
            AddPREComponent<PREModelRendererComponent>();
            auto &modelComponent = *AddPREComponent<SimpleModelComponent>();
            modelComponent.vertexShaderPath = "/shaders/simplevertex.vs";
            modelComponent.fragmentShaderPath = "/shaders/simplefragment.fs";
            modelComponent.meshPath = "/models/vampire_a_lusth/vampire_a_lusth.dae";
            modelComponent.albedoPath = "/models/vampire_a_lusth/textures/Vampire_diffuse.png";
            auto pTransform = GetPREComponent<PRETransformComponent>();
            pTransform->SetLocalScale(glm::vec3(4.0f));
            pTransform->SetPosition(glm::vec3(0, -5, 0));
        }
    } linkTemplate;

    class : public PREGameObjectTemplate
    {
    protected:
        void OnInstantiateTemplate() override
        {
            AddPREComponent<PREModelRendererComponent>();
            auto& modelComponent = *AddPREComponent<SimpleModelComponent>();
            modelComponent.vertexShaderPath = "/shaders/simplevertex.vs";
            modelComponent.fragmentShaderPath = "/shaders/simplefragment.fs";
            modelComponent.meshPath = "/models/backpack/backpack.obj";
            modelComponent.albedoPath = "/models/backpack/diffuse.jpg";
        }
    } backpackTemplate;

    class : public PREGameObjectTemplate
    {
    protected:
        void OnInstantiateTemplate() override
        {
            AddPREComponent<CameraControllerComponent>();
            AddPREComponent<PRECameraComponent>();
        }
    } cameraTemplate;

    auto& camera = applicationContext.world.Instantiate(cameraTemplate);
    auto pCameraComponent = camera.GetComponent<PRECameraComponent>();
    
    auto& backpack = applicationContext.world.Instantiate(backpackTemplate);
    backpack.GetComponent<PREModelRendererComponent>()->SetCameraComponent(pCameraComponent);
    auto pBackpackTransform = backpack.GetComponent<PRETransformComponent>();

    {
        auto& linkA = applicationContext.world.Instantiate(linkTemplate);
        auto pLinkATransform = linkA.GetComponent<PRETransformComponent>();
        pLinkATransform->SetPosition(
            pLinkATransform->GetPosition() + glm::vec3(-4, 0, 0)
        );
        //pLinkATransform->SetParent(pBackpackTransform, true);
        linkA.GetComponent<PREModelRendererComponent>()->SetCameraComponent(pCameraComponent);
    }

    {
        auto& linkB = applicationContext.world.Instantiate(linkTemplate);
        auto pLinkBTransform = linkB.GetComponent<PRETransformComponent>();
        pLinkBTransform->SetPosition(
            pLinkBTransform->GetPosition() + glm::vec3(4, 0, 0)
        );
        //pLinkBTransform->SetParent(pBackpackTransform, true);
        linkB.GetComponent<PREModelRendererComponent>()->SetCameraComponent(pCameraComponent);
    }
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
                1024,
                768
            ),
            PRETimeConfig(),
            PREWorldConfig(),
            PREAssetManagerConfig("assets"),
            OnInitialize,
            OnShutdown
        )
    );
    return 0;
}