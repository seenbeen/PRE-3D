#include <iostream>

#include <include/core.h>

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
        auto euler = _transform->GetEuler() + glm::vec3(30) * GetTime().GetDeltaTime();
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
            glm::vec3(0, 0, -1)
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