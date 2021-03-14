#ifdef __PRE_DEBUG__
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

        if (!animationPath.empty())
        {
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
    }

    void OnUpdate() override
    {
         auto euler = _transform->GetLocalEuler() + glm::vec3(0, 60, 0) * GetTime().GetDeltaTime();
         //_transform->SetLocalEuler(euler);
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
        if (_pAnim != nullptr)
        {
            GetRendering().DestroyAnimation(*_pAnim);
        }
        if (_pAnimator != nullptr)
        {
            GetRendering().DestroyAnimator(*_pAnimator);
        }
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

class FlyCamControllerComponent : public PREGameObjectComponent
{
public:
    float speed = 5.0f;
    float rotationSpeed = 30.0f;

protected:
    void OnStart() override
    {
        _pTransform = gameObject().GetComponent<PRETransformComponent>();
        _pInput = &GetInput();
        _pTime = &GetTime();
        _pInput->LockMouse(true);
    }

    void OnUpdate() override
    {
        glm::vec3 delta;

        // flycam controls
        if (_pInput->KeyState(PREKeyCode::S))
        {
            delta -= _pTransform->GetForward();
        }
        else if (_pInput->KeyState(PREKeyCode::W))
        {
            delta += _pTransform->GetForward();
        }
        if (_pInput->KeyState(PREKeyCode::A))
        {
            delta -= _pTransform->GetRight();
        }
        else if (_pInput->KeyState(PREKeyCode::D))
        {
            delta += _pTransform->GetRight();
        }
        if (_pInput->KeyState(PREKeyCode::Q))
        {
            delta -= _pTransform->GetUp();
        }
        else if (_pInput->KeyState(PREKeyCode::E))
        {
            delta += _pTransform->GetUp();
        }

        if (glm::length(delta))
        {
            _pTransform->SetPosition(
                _pTransform->GetPosition() + speed * delta * _pTime->GetDeltaTime()
            );
        }

        int mdx, mdy;
        _pInput->MouseMotion(mdx, mdy);
        if (mdx || mdy)
        {
            auto dx = (float)mdx / HALF_WINDOW_WIDTH;
            auto dy = -(float)mdy / HALF_WINDOW_HEIGHT;

            _pTransform->SetEuler(
                _pTransform->GetEuler() + glm::vec3(dy, dx, 0) * rotationSpeed
            );
        }
    }

private:
    static const int HALF_WINDOW_WIDTH = 1024 / 2;
    static const int HALF_WINDOW_HEIGHT = 768 / 2;

    PRETransformComponent* _pTransform = nullptr;
    PREInput* _pInput = nullptr;
    PRETime* _pTime = nullptr;
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
        cameraComponent.SetRenderTexture(&GetRendering().GetScreenRenderTexture());
        cameraComponent.SetFarClippingPlane(1000);
        cameraComponent.SetSize(75);
        _pSkybox = &GetAssetManager().LoadSkyBox(
            GetAssetManager().rootAssetPath + skyBoxRightPath,
            GetAssetManager().rootAssetPath + skyBoxLeftPath,
            GetAssetManager().rootAssetPath + skyBoxTopPath,
            GetAssetManager().rootAssetPath + skyBoxBottomPath,
            GetAssetManager().rootAssetPath + skyBoxFrontPath,
            GetAssetManager().rootAssetPath + skyBoxBackPath
        );
        cameraComponent.SetSkyBox(_pSkybox);
        _pTransform = gameObject().GetComponent<PRETransformComponent>();
        _pInput = &GetInput();
    }

    void OnUpdate() override
    {
        if (_pInput->ApplicationHasQuit() || _pInput->KeyPressed(PREKeyCode::ESCAPE))
        {
            std::cout << "Application Quitting..." << std::endl;
            Quit();
        }
        if (_pInput->MouseButtonLeftPressed())
        {
            int x, y;
            _pInput->MousePosition(x, y);
            std::cout << "PRESSED AT (" << x << ", " << y << ")" << std::endl;
        }
        if (_pInput->MouseButtonLeftReleased())
        {
            int x, y;
            _pInput->MousePosition(x, y);
            std::cout << "RELEASED AT (" << x << ", " << y << ")" << std::endl;
        }
        if (_pInput->KeyPressed(PREKeyCode::SPACE))
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
    PRETransformComponent* _pTransform = nullptr;
    PREInput* _pInput = nullptr;
};

class VampireTemplate : public PREGameObjectTemplate
{
public:
    string animationPath;

protected:
    void OnInstantiateTemplate() override
    {
        AddPREComponent<PREModelRendererComponent>();
        AddPREComponent<PREAnimatorComponent>();
        auto& modelComponent = *AddPREComponent<SimpleModelComponent>();
        modelComponent.vertexShaderPath = "/shaders/skinnedvertex.vs";
        modelComponent.fragmentShaderPath = "/shaders/simplefragment.fs";
        modelComponent.meshPath = "/models/vampire_a_lusth/vampire_a_lusth.dae";
        modelComponent.skeletonPath = "/models/vampire_a_lusth/vampire_a_lusth.dae";
        modelComponent.animationPath = animationPath;
        modelComponent.animationKey = "";
        modelComponent.diffusePath = "/models/vampire_a_lusth/textures/Vampire_diffuse.png";
        modelComponent.emissionPath = "/models/vampire_a_lusth/textures/Vampire_emission.png";
        modelComponent.normalPath = "/models/vampire_a_lusth/textures/Vampire_normal.png";
        modelComponent.specularPath = "/models/vampire_a_lusth/textures/Vampire_specular.png";
    }
} ;

class CameraTemplate : public PREGameObjectTemplate
{
protected:
    void OnInstantiateTemplate() override
    {
        AddPREComponent<PRECameraComponent>();
        AddPREComponent<FlyCamControllerComponent>();
        auto& cameraComponent = *AddPREComponent<CameraControllerComponent>();
        cameraComponent.skyBoxRightPath = "/skyboxes/Night MoonBurst/Right+X.png";
        cameraComponent.skyBoxLeftPath = "/skyboxes/Night MoonBurst/Left-X.png";
        cameraComponent.skyBoxTopPath = "/skyboxes/Night MoonBurst/Top+Y.png";
        cameraComponent.skyBoxBottomPath = "/skyboxes/Night MoonBurst/Bottom-Y.png";
        cameraComponent.skyBoxFrontPath = "/skyboxes/Night MoonBurst/Front+Z.png";
        cameraComponent.skyBoxBackPath = "/skyboxes/Night MoonBurst/Back-Z.png";
        GetPREComponent<PRETransformComponent>()->SetPosition(
            glm::vec3(0, 1, 2.5)
        );
    }
};

void OnInitialize(PREApplicationContext& applicationContext)
{
    std::cout << "ON INITIALIZE" << std::endl;

    VampireTemplate capoeiraTemplate, thrillerTemplate;
    capoeiraTemplate.animationPath = "/animations/mixamo/Capoeira.dae";
    thrillerTemplate.animationPath = "/animations/mixamo/Thriller Part 4.dae";

    CameraTemplate cameraTemplate;

    auto& sceneRoot = applicationContext.world.Instantiate();
    auto& sceneRootTransform = *sceneRoot.GetComponent<PRETransformComponent>();

    auto& camera = applicationContext.world.Instantiate(cameraTemplate);
    auto& cameraComponent = *camera.GetComponent<PRECameraComponent>();
    auto& cameraTransform = *camera.GetComponent<PRETransformComponent>();
    cameraTransform.SetParent(&sceneRootTransform, true);

    auto& vampireA = applicationContext.world.Instantiate(capoeiraTemplate);
    auto& vampireATransform = *vampireA.GetComponent<PRETransformComponent>();
    vampireATransform.SetPosition(
        vampireATransform.GetPosition() + glm::vec3(-1, 0, 0)
    );
    vampireATransform.SetParent(&sceneRootTransform, true);
    vampireA.GetComponent<PREModelRendererComponent>()->SetCameraComponent(&cameraComponent);

    auto& vampireB = applicationContext.world.Instantiate(thrillerTemplate);
    auto& vampireBTransform = *vampireB.GetComponent<PRETransformComponent>();
    vampireBTransform.SetPosition(
        vampireBTransform.GetPosition() + glm::vec3(1, 0, -2)
    );
    vampireBTransform.SetParent(&sceneRootTransform, true);
    vampireB.GetComponent<PREModelRendererComponent>()->SetCameraComponent(&cameraComponent);
    
    sceneRootTransform.SetEuler(glm::vec3(0, 180, 0));
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
    return 0;
}