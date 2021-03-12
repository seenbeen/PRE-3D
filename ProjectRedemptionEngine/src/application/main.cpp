#ifdef __PRE_DEBUG__
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

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
    string skeletonPath;
    string animationPath;
    string animationKey;
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

        if (!skeletonPath.empty())
        {
            _pSkeleton = &GetAssetManager().LoadSkeleton(
                GetAssetManager().rootAssetPath + skeletonPath
            );
        }

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
        modelRendererComponent.SetSkeleton(_pSkeleton);
        modelRendererComponent.SetMaterial(_pMaterial);

        _pAnim = &GetAssetManager().LoadAnimation(
            GetAssetManager().rootAssetPath + animationPath,
            animationKey
        );

        PREAnimatorConfig animatorConfig;
        animatorConfig.AddState(
            "default",
            [](PREAnimatorComponent::Controller& controller) {},
            *_pAnim
        );

        _pAnimator = &GetRendering().CreateAnimator(animatorConfig);

        auto pAnimatorComponent = gameObject().GetComponent<PREAnimatorComponent>();
        pAnimatorComponent->SetAnimator(
            _pAnimator,
            "default",
            nullptr,
            0,
            1,
            false
        );
        pAnimatorComponent->SetSkeleton(_pSkeleton);
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
        if (_pSkeleton != nullptr)
        {
            GetRendering().DestroySkeleton(*_pSkeleton);
        }
        GetRendering().DestroyTexture(*_pDiffuseTexture);
        GetRendering().DestroyTexture(*_pEmissionTexture);
        GetRendering().DestroyTexture(*_pNormalTexture);
        GetRendering().DestroyTexture(*_pSpecularTexture);
        GetRendering().DestroyMaterial(*_pMaterial);
        GetRendering().DestroyAnimation(*_pAnim);
        GetRendering().DestroyAnimator(*_pAnimator);
    }

private:
    PRETransformComponent* _transform = nullptr;
    PREShader* _pShader = nullptr;
    PREMesh* _pMesh = nullptr;
    PRESkeleton* _pSkeleton = nullptr;
    PRETexture* _pDiffuseTexture = nullptr;
    PRETexture* _pEmissionTexture = nullptr;
    PRETexture* _pNormalTexture = nullptr;
    PRETexture* _pSpecularTexture = nullptr;
    PREMaterial* _pMaterial = nullptr;
    PREAnimation* _pAnim = nullptr;
    PREAnimator* _pAnimator = nullptr;
};

class CameraControllerComponent : public PREGameObjectComponent
{
public:
    string skyBoxRightPath;
    string skyBoxLeftPath;
    string skyBoxTopPath;
    string skyBoxBottomPath;
    string skyBoxFrontPath;
    string skyBoxBackPath;

protected:
    void OnStart() override
    {
        auto& cameraComponent = *gameObject().GetComponent<PRECameraComponent>();
        cameraComponent.SetKind(PRECameraComponent::Kind::PERSPECTIVE);
        cameraComponent.SetRenderTexture(&GetRendering().rootRenderTexture);
        cameraComponent.SetFarClippingPlane(1000);
        cameraComponent.SetSize(90);
        _pSkybox = &GetAssetManager().LoadSkyBox(
            GetAssetManager().rootAssetPath + skyBoxRightPath,
            GetAssetManager().rootAssetPath + skyBoxLeftPath,
            GetAssetManager().rootAssetPath + skyBoxTopPath,
            GetAssetManager().rootAssetPath + skyBoxBottomPath,
            GetAssetManager().rootAssetPath + skyBoxFrontPath,
            GetAssetManager().rootAssetPath + skyBoxBackPath
        );
        cameraComponent.SetSkyBox(_pSkybox);
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

    void OnDestroy() override
    {
        GetRendering().DestroySkyBox(*_pSkybox);
    }

private:
    PRESkyBox* _pSkybox = nullptr;
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
            modelComponent.vertexShaderPath = "/shaders/skinnedvertex.vs";
            modelComponent.fragmentShaderPath = "/shaders/simplefragment.fs";
            modelComponent.meshPath = "/models/vampire_a_lusth/vampire_a_lusth.dae";
            modelComponent.skeletonPath = "/models/vampire_a_lusth/vampire_a_lusth.dae";
            modelComponent.animationPath = "/animations/mixamo/Running.dae";
            modelComponent.animationKey = "";
            modelComponent.diffusePath = "/models/vampire_a_lusth/textures/Vampire_diffuse.png";
            modelComponent.emissionPath = "/models/vampire_a_lusth/textures/Vampire_emission.png";
            modelComponent.normalPath = "/models/vampire_a_lusth/textures/Vampire_normal.png";
            modelComponent.specularPath = "/models/vampire_a_lusth/textures/Vampire_specular.png";
            AddPREComponent<PREAnimatorComponent>();
            auto pTransform = GetPREComponent<PRETransformComponent>();
            pTransform->SetLocalScale(glm::vec3(4.0));
            pTransform->SetPosition(glm::vec3(0, -5, 0));
        }
    } vampireTemplate;

    class : public PREGameObjectTemplate
    {
    protected:
        void OnInstantiateTemplate() override
        {
            AddPREComponent<PREModelRendererComponent>();
            auto& modelComponent = *AddPREComponent<SimpleModelComponent>();
            modelComponent.vertexShaderPath = "/shaders/skinnedvertex.vs";
            modelComponent.fragmentShaderPath = "/shaders/simplefragment.fs";
            modelComponent.meshPath = "/models/vampire_a_lusth/vampire_a_lusth.dae";
            modelComponent.skeletonPath = "/models/vampire_a_lusth/vampire_a_lusth.dae";
            modelComponent.animationPath = "/animations/mixamo/Breathing Idle.dae";
            modelComponent.animationKey = "";
            modelComponent.diffusePath = "/models/vampire_a_lusth/textures/Vampire_diffuse.png";
            modelComponent.emissionPath = "/models/vampire_a_lusth/textures/Vampire_emission.png";
            modelComponent.normalPath = "/models/vampire_a_lusth/textures/Vampire_normal.png";
            modelComponent.specularPath = "/models/vampire_a_lusth/textures/Vampire_specular.png";
            AddPREComponent<PREAnimatorComponent>();
            auto pTransform = GetPREComponent<PRETransformComponent>();
            pTransform->SetLocalScale(glm::vec3(4.0));
            pTransform->SetPosition(glm::vec3(0, -5, 0));
        }
    } vampireTemplate2;

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
            AddPREComponent<PRECameraComponent>();
            auto& cameraComponent = *AddPREComponent<CameraControllerComponent>();
            cameraComponent.skyBoxRightPath = "/skyboxes/Night MoonBurst/Right+X.png";
            cameraComponent.skyBoxLeftPath = "/skyboxes/Night MoonBurst/Left-X.png";
            cameraComponent.skyBoxTopPath = "/skyboxes/Night MoonBurst/Top+Y.png";
            cameraComponent.skyBoxBottomPath = "/skyboxes/Night MoonBurst/Bottom-Y.png";
            cameraComponent.skyBoxFrontPath = "/skyboxes/Night MoonBurst/Front+Z.png";
            cameraComponent.skyBoxBackPath = "/skyboxes/Night MoonBurst/Back-Z.png";
        }
    } cameraTemplate;

    auto& camera = applicationContext.world.Instantiate(cameraTemplate);
    auto pCameraComponent = camera.GetComponent<PRECameraComponent>();
    
    //auto& backpack = applicationContext.world.Instantiate(backpackTemplate);
    //backpack.GetComponent<PREModelRendererComponent>()->SetCameraComponent(pCameraComponent);
    //auto pBackpackTransform = backpack.GetComponent<PRETransformComponent>();

    {
        auto& vampireA = applicationContext.world.Instantiate(vampireTemplate);
        auto pvampireATransform = vampireA.GetComponent<PRETransformComponent>();
        pvampireATransform->SetPosition(
            pvampireATransform->GetPosition() + glm::vec3(-4, 0, 0)
        );
        //pLinkATransform->SetParent(pBackpackTransform, true);
        vampireA.GetComponent<PREModelRendererComponent>()->SetCameraComponent(pCameraComponent);
    }
    for (int i = 0; i < 1; ++i)
    {
        auto& vampireB = applicationContext.world.Instantiate(vampireTemplate2);
        auto pvampireBTransform = vampireB.GetComponent<PRETransformComponent>();
        pvampireBTransform->SetPosition(
            pvampireBTransform->GetPosition() + glm::vec3(4 + i, 0, 0)
        );
        //pLinkBTransform->SetParent(pBackpackTransform, true);
        vampireB.GetComponent<PREModelRendererComponent>()->SetCameraComponent(pCameraComponent);
    }
}

void OnShutdown(PREApplicationContext& applicationContext)
{
    std::cout << "ON SHUTDOWN" << std::endl;
}

int main(int argc, char *argv[])
{
#ifdef __PRE_DEBUG__
    _CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
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
    }
#ifdef __PRE_DEBUG__
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}