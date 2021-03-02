#include <iostream>

#include <include/core.h>
#include <include/modules/gameobject.h>

using PRE::Core::PREApplication;
using PRE::Core::PREApplicationConfig;
using PRE::Core::PREApplicationContext;

using PRE::Core::PRETransformComponent;

using PRE::Core::PREGameObjectTemplate;
using PRE::Core::PREGameObjectComponent;

class FooComponent : public PREGameObjectComponent
{
protected:
    void OnStart() override
    {
        std::cout << "Start!" << std::endl;
        _transform = gameObject().GetComponent<PRETransformComponent>();
        _derp = 0;
    }

    void OnUpdate() override
    {
        _derp += GetTime()->GetDeltaTime();
        if (_derp >= 3.0f)
        {
            std::cout << "3 seconds, i'm out." << std::endl;
            std::cout << _transform->GetEuler().x << std::endl;
            gameObject().Destroy(gameObject());
        }
    }

    void OnDestroy() override
    {
        std::cout << "Destroy!" << std::endl;
    }

private:
    PRETransformComponent* _transform = nullptr;
    float _derp = 0;
};

class BarComponent : public PREGameObjectComponent
{
protected:
    void OnUpdate() override
    {
        if (GetInput()->ApplicationHasQuit())
        {
            std::cout << "Application Quitting..." << std::endl;
            Quit();
        }
        if (GetInput()->MouseButtonLeftPressed())
        {
            int x, y;
            GetInput()->MousePosition(x, y);
            std::cout << "PRESSED AT (" << x << ", " << y << ")" << std::endl;
        }
        if (GetInput()->MouseButtonLeftReleased())
        {
            int x, y;
            GetInput()->MousePosition(x, y);
            std::cout << "RELEASED AT (" << x << ", " << y << ")" << std::endl;
        }
        if (GetInput()->KeyPressed(SDL_SCANCODE_SPACE))
        {
            std::cout << "~" << 1 / GetTime()->GetDeltaTime() << " FPS." << std::endl;
        }
    }
};

void OnInitialize(PREApplicationContext* applicationContext)
{
    std::cout << "ON INITIALIZE" << std::endl;

    class : public PREGameObjectTemplate
    {
    protected:
        void OnInstantiate() override
        {
            AddPREComponent<PRETransformComponent>();
            AddPREComponent<FooComponent>();
        }
    } fooTemplate;

    class : public PREGameObjectTemplate
    {
    protected:
        void OnInstantiate() override
        {
            AddPREComponent<BarComponent>();
        }
    } barTemplate;

    applicationContext->world->Instantiate(fooTemplate);
    applicationContext->world->Instantiate(barTemplate);
}

void OnShutdown(PREApplicationContext* applicationContext)
{
    std::cout << "ON SHUTDOWN" << std::endl;
}

int main(int argc, char *argv[])
{
    PREApplication::Run(
        PREApplicationConfig(
            (int) PREApplicationConfig::Options::USE_TIME |
            (int) PREApplicationConfig::Options::USE_RENDERING,
            OnInitialize,
            OnShutdown
        )
    );
    return 0;
}