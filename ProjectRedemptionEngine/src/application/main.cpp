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
            Quit();
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

class FooTemplate : public PREGameObjectTemplate
{
protected:
    void OnInstantiate() override
    {
        AddPREComponent<PRETransformComponent>();
        AddPREComponent<FooComponent>();
    }
};

void OnInitialize(PREApplicationContext* applicationContext)
{
    std::cout << "ON INITIALIZE" << std::endl;
    FooTemplate fooTemplate;
    auto& obj = applicationContext->world->Instantiate(fooTemplate);
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