#include <iostream>

#include <include/core.h>
#include <include/modules/gameobject.h>

using PRE::Core::PREApplicationConfig;
using PRE::Core::PREApplicationContext;
using PRE::Core::PREApplication;

using PRE::Core::TransformComponent;

using PRE::GameObjectModule::GameObjectTemplate;
using PRE::GameObjectModule::GameObjectComponent;

class FooComponent : public GameObjectComponent
{
public:
    PREApplicationContext* applicationContext = nullptr;

protected:
    void OnStart() override
    {
        std::cout << "Start!" << std::endl;
        _transform = gameObject().GetComponent<TransformComponent>();
        _derp = 0;
    }

    void OnUpdate() override
    {
        _derp += applicationContext->time->GetDeltaTime();
        if (_derp >= 3.0f)
        {
            std::cout << "3 seconds, i'm out." << std::endl;
            std::cout << _transform->GetEuler().x << std::endl;
            applicationContext->Quit();
        }
    }

    void OnDestroy() override
    {
        std::cout << "Destroy!" << std::endl;
    }

private:
    TransformComponent* _transform = nullptr;
    float _derp = 0;
};

class FooTemplate : public GameObjectTemplate
{
protected:
    void OnInstantiate() override
    {
        AddComponent<PRE::Core::TransformComponent>();
        AddComponent<FooComponent>();
    }
};

void OnInitialize(PREApplicationContext* applicationContext)
{
    std::cout << "ON INITIALIZE" << std::endl;
    FooTemplate fooTemplate;
    auto& obj = applicationContext->world->Instantiate(fooTemplate);
    obj.GetComponent<FooComponent>()->applicationContext = applicationContext;
}

void OnShutdown(PREApplicationContext* applicationContext)
{
    std::cout << "ON SHUTDOWN" << std::endl;
}

int main(int argc, char *argv[])
{
    PREApplication::Run(
        PREApplicationConfig(
            (int)PREApplicationConfig::Options::USE_TIME,
            OnInitialize,
            OnShutdown
        )
    );
    return 0;
}