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
    string diffusePath;
    string emissionPath;
    string normalPath;
    string specularPath;

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

        _pDiffuseTexture = &GetAssetManager().LoadTexture(
            GetAssetManager().rootAssetPath + diffusePath
        );

        _pEmissionTexture = &GetAssetManager().LoadTexture(
            GetAssetManager().rootAssetPath + emissionPath
        );

        _pNormalTexture = &GetAssetManager().LoadTexture(
            GetAssetManager().rootAssetPath + normalPath
        );

        _pSpecularTexture = &GetAssetManager().LoadTexture(
            GetAssetManager().rootAssetPath + specularPath
        );

        _pMaterial = &GetRendering().CreateMaterial();
        _pMaterial->SetShader(_pShader);
        _pMaterial->SetTextureBinding(_pDiffuseTexture, 0);
        _pMaterial->SetTextureBinding(_pEmissionTexture, 1);
        _pMaterial->SetTextureBinding(_pNormalTexture, 2);
        _pMaterial->SetTextureBinding(_pSpecularTexture, 3);

        _pShader->SetInt("diffuseSampler", 0);
        _pShader->SetInt("emissionSampler", 1);
        _pShader->SetInt("normalSampler", 2);
        _pShader->SetInt("specularSampler", 3);

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
        GetRendering().DestroyShader(*_pShader);
        GetRendering().DestroyMesh(*_pMesh);
        GetRendering().DestroyTexture(*_pDiffuseTexture);
        GetRendering().DestroyTexture(*_pEmissionTexture);
        GetRendering().DestroyTexture(*_pNormalTexture);
        GetRendering().DestroyTexture(*_pSpecularTexture);
        GetRendering().DestroyMaterial(*_pMaterial);
    }

private:
    PRETransformComponent* _transform = nullptr;
    PREShader* _pShader = nullptr;
    PREMesh* _pMesh = nullptr;
    PRETexture* _pDiffuseTexture = nullptr;
    PRETexture* _pEmissionTexture = nullptr;
    PRETexture* _pNormalTexture = nullptr;
    PRETexture* _pSpecularTexture = nullptr;
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
            modelComponent.diffusePath = "/models/vampire_a_lusth/textures/Vampire_diffuse.png";
            modelComponent.emissionPath = "/models/vampire_a_lusth/textures/Vampire_emission.png";
            modelComponent.normalPath = "/models/vampire_a_lusth/textures/Vampire_normal.png";
            modelComponent.specularPath = "/models/vampire_a_lusth/textures/Vampire_specular.png";
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
            modelComponent.diffusePath = "/models/backpack/diffuse.jpg";
            modelComponent.emissionPath = "/models/backpack/roughness.jpg";
            modelComponent.normalPath = "/models/backpack/normal.png";
            modelComponent.specularPath = "/models/backpack/specular.jpg";
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
    
    //auto& backpack = applicationContext.world.Instantiate(backpackTemplate);
    //backpack.GetComponent<PREModelRendererComponent>()->SetCameraComponent(pCameraComponent);
    //auto pBackpackTransform = backpack.GetComponent<PRETransformComponent>();

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