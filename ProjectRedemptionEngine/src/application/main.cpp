#ifdef __PRE_DEBUG__
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <iostream>
#include <string>

#include <glm/glm.hpp>

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
        _pMaterial->SetTextureBinding(_pDiffuseTexture, 3);
        _pMaterial->SetTextureBinding(_pEmissionTexture, 4);
        _pMaterial->SetTextureBinding(_pNormalTexture, 5);
        _pMaterial->SetTextureBinding(_pSpecularTexture, 6);

        _pShader->SetInt("diffuseSampler", 3);
        _pShader->SetInt("emissionSampler", 4);
        _pShader->SetInt("normalSampler", 5);
        _pShader->SetInt("specularSampler", 6);
        _pShader->SetBackFaceCulling(false);

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
         //auto euler = _transform->GetLocalEuler() + glm::vec3(0, 60, 0) * GetTime().GetDeltaTime();
         //_transform->SetLocalEuler(euler);
    }

    void OnDestroy() override
    {
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

class FloorModelComponent : public PREGameObjectComponent
{
protected:
    void OnStart() override
    {
        const unsigned int VERTEX_COUNT = 4u;
        const glm::vec3 VERTICES[]
        {
            glm::vec3(-1,  1, 0),
            glm::vec3(-1, -1, 0),
            glm::vec3( 1, -1, 0),
            glm::vec3( 1,  1, 0)
        };
        const glm::vec2 UVS[]
        {
            glm::vec2(0, 4),
            glm::vec2(0, 0),
            glm::vec2(4, 0),
            glm::vec2(4, 4)
        };
        const glm::vec3 NORMALS[]
        {
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 1)
        };
        const glm::vec3 TANGENTS[]
        {
            glm::vec3(1, 0, 0),
            glm::vec3(1, 0, 0),
            glm::vec3(1, 0, 0),
            glm::vec3(1, 0, 0)
        };
        const glm::vec3 BITANGENTS[]
        {
            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 0)
        };
        unsigned int TRIANGLE_ELEMENT_COUNT = 6u;
        const unsigned int TRIANGLE_ELEMENTS[]
        {
            0u, 1u, 2u, 0u, 2u, 3u
        };

        _pShader = &GetAssetManager().LoadShader(
            GetAssetManager().rootAssetPath + "/shaders/simplevertex.vs",
            GetAssetManager().rootAssetPath + "/shaders/simplefragment.fs"
        );

        _pMesh = &GetRendering().CreateMesh(
            VERTEX_COUNT,
            VERTICES,
            NORMALS,
            TANGENTS,
            BITANGENTS,
            UVS,
            nullptr,
            nullptr,
            TRIANGLE_ELEMENT_COUNT,
            TRIANGLE_ELEMENTS
        );

        _pDiffuseTexture = &GetAssetManager().LoadTexture(
            GetAssetManager().rootAssetPath + "/models/cobble/diffuse.png"
        );

        _pEmissionTexture = &GetAssetManager().LoadTexture(
            GetAssetManager().rootAssetPath + "/models/cobble/diffuse.png"
        );

        _pNormalTexture = &GetAssetManager().LoadTexture(
            GetAssetManager().rootAssetPath + "/models/cobble/normal.png"
        );

        _pSpecularTexture = &GetAssetManager().LoadTexture(
            GetAssetManager().rootAssetPath + "/models/cobble/specular.png"
        );

        _pMaterial = &GetRendering().CreateMaterial();
        _pMaterial->SetShader(_pShader);

        _pMaterial->SetTextureBinding(_pDiffuseTexture, 3);
        _pMaterial->SetTextureBinding(_pEmissionTexture, 4);
        _pMaterial->SetTextureBinding(_pNormalTexture, 5);
        _pMaterial->SetTextureBinding(_pSpecularTexture, 6);

        _pShader->SetInt("diffuseSampler", 3);
        _pShader->SetInt("emissionSampler", 4);
        _pShader->SetInt("normalSampler", 5);
        _pShader->SetInt("specularSampler", 6);
        auto& modelRendererComponent = *gameObject().GetComponent<PREModelRendererComponent>();
        modelRendererComponent.SetMesh(_pMesh);
        modelRendererComponent.SetMaterial(_pMaterial);
        modelRendererComponent.SetCastsShadows(false);
    }

    void OnDestroy() override
    {
        GetRendering().DestroyShader(*_pShader);
        GetRendering().DestroyMesh(*_pMesh);
        GetRendering().DestroyTexture(*_pDiffuseTexture);
        GetRendering().DestroyTexture(*_pEmissionTexture);
        GetRendering().DestroyTexture(*_pNormalTexture);
        GetRendering().DestroyTexture(*_pSpecularTexture);
        GetRendering().DestroyMaterial(*_pMaterial);
    }

private:
    PREShader* _pShader = nullptr;
    PREMesh* _pMesh = nullptr;

    PRETexture* _pDiffuseTexture = nullptr;
    PRETexture* _pEmissionTexture = nullptr;
    PRETexture* _pNormalTexture = nullptr;
    PRETexture* _pSpecularTexture = nullptr;

    PREMaterial* _pMaterial = nullptr;
};

class LightCubeComponent : public PREGameObjectComponent
{
public:
    glm::vec3 color = glm::vec3(1);

protected:
    void OnStart() override
    {
        const string VERT_SHADER =
            "#version 330 core\n"
            "layout (location = 0) in vec3 iPos;\n"
            "\n"
            "uniform mat4 PRE_MODEL_MATRIX;\n"
            "uniform mat4 PRE_VIEW_MATRIX;\n"
            "uniform mat4 PRE_PROJECTION_MATRIX;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    vec4 pos = PRE_PROJECTION_MATRIX * PRE_VIEW_MATRIX * PRE_MODEL_MATRIX * vec4(iPos, 1.0f);\n"
            "    gl_Position = pos;\n"
            "}\n";
        const string FRAG_SHADER =
            "#version 330 core\n"
            "\n"
            "out vec4 FragColor;\n"
            "\n"
            "uniform vec3 LightColor;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    FragColor = vec4(LightColor, 1.0f);\n"
            "}\n";
        const unsigned int N_VERTS = 8;
        const glm::vec3 VERTS[] {
            glm::vec3(-1, -1, 1),
            glm::vec3(1, -1, 1),
            glm::vec3(1, 1, 1),
            glm::vec3(-1, 1, 1),
            glm::vec3(-1, -1, -1),
            glm::vec3(1, -1, -1),
            glm::vec3(1, 1, -1),
            glm::vec3(-1, 1, -1),
        };
        static const unsigned int N_ELEMS = 36;
        const unsigned int ELEMS[]{
            6u, 2u, 5u, 2u, 1u, 5u, // right
            3u, 7u, 0u, 7u, 4u, 0u, // left
            3u, 2u, 7u, 2u, 6u, 7u, // top
            1u, 0u, 5u, 0u, 4u, 5u, // bottom
            7u, 6u, 4u, 6u, 5u, 4u, // front
            2u, 3u, 1u, 3u, 0u, 1u // back
        };

        _pShader = &GetRendering().CreateShader(VERT_SHADER, FRAG_SHADER);

        _pMesh = &GetRendering().CreateMesh(N_VERTS, VERTS, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, N_ELEMS, ELEMS);

        _pMaterial = &GetRendering().CreateMaterial();
        _pMaterial->SetShader(_pShader);

        auto& modelRendererComponent = *gameObject().GetComponent<PREModelRendererComponent>();
        modelRendererComponent.SetMesh(_pMesh);
        modelRendererComponent.SetMaterial(_pMaterial);
        modelRendererComponent.SetCastsShadows(false);

        gameObject().GetComponent<PRETransformComponent>()->SetScale(glm::vec3(0.1f));
    }

    void OnUpdate() override
    {
        _pShader->SetVec3("LightColor", color);
    }

    void OnDestroy() override
    {
        GetRendering().DestroyShader(*_pShader);
        GetRendering().DestroyMesh(*_pMesh);
        GetRendering().DestroyMaterial(*_pMaterial);
    }

private:
    PREShader* _pShader = nullptr;
    PREMesh* _pMesh = nullptr;
    PREMaterial* _pMaterial = nullptr;
};

class FlyCamControllerComponent : public PREGameObjectComponent
{
public:
    float speed = 3.5f;
    float rotationSpeed = 60.0f;
    bool enabled = true;
    PRESpotLightComponent* _pSpotLightComponent = nullptr;

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
        if (_pInput->KeyPressed(PREKeyCode::TAB))
        {
            enabled = !enabled;
        }

        if (!enabled)
        {
            return;
        }

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
            auto dx = -(float)mdx / HALF_WINDOW_WIDTH;
            auto dy = -(float)mdy / HALF_WINDOW_HEIGHT;

            // TODO: framerate and viewport independent rotation
            _pTransform->SetLocalEuler(
                _pTransform->GetLocalEuler() + glm::vec3(dy, dx, 0) * rotationSpeed
            );
        }

        if (_pInput->KeyPressed(PREKeyCode::O) && _pSpotLightComponent != nullptr)
        {
            _pSpotLightComponent->SetColor(glm::vec3(1.0f));
        }
        if (_pInput->KeyPressed(PREKeyCode::P) && _pSpotLightComponent != nullptr)
        {
            _pSpotLightComponent->SetColor(glm::vec3(0.0f));
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
        cameraComponent.SetFarClippingPlane(1000);
        cameraComponent.SetSize(75);
        cameraComponent.SetAspectRatio(4.0f / 1.5f);
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
        if (!gameObject().GetComponent<FlyCamControllerComponent>()->enabled)
        {
            return;
        }
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

class BufferDisplayScreenComponent : public PREGameObjectComponent
{
public:
    PRERenderTexture* pRenderTexture = nullptr;

protected:
    void OnStart() override
    {
        const string SAMPLER_KEY = "bufferSampler";
        const int SAMPLER_UNIT = 0;
        const string VERTEX_SHADER_SOURCE =
            "#version 330 core\n"
            "layout (location = 0) in vec3 iPos;\n"
            "layout (location = 4) in vec2 iUv;\n"
            "\n"
            "out vec2 TexCoord;\n"
            "\n"
            "uniform mat4 PRE_MODEL_MATRIX;\n"
            "uniform mat4 PRE_VIEW_MATRIX;\n"
            "uniform mat4 PRE_PROJECTION_MATRIX;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    TexCoord = iUv;\n"
            "    gl_Position = PRE_PROJECTION_MATRIX * PRE_VIEW_MATRIX * PRE_MODEL_MATRIX * vec4(iPos, 1.0f);\n"
            "}\n";

        const string FRAGMENT_SHADER_SOURCE =
            "#version 330 core\n"
            "in vec2 TexCoord;\n"
            "\n"
            "out vec4 FragColor;\n"
            "\n"
            "uniform sampler2D bufferSampler;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    FragColor = texture(bufferSampler, TexCoord);\n"
            "}\n";

        const unsigned int VERTEX_COUNT = 4u;
        const glm::vec3 VERTICES[]
        {
            glm::vec3(-1, 0, 0),
            glm::vec3(1, 0, 0),
            glm::vec3(1, 1, 0),
            glm::vec3(-1, 1, 0)
        };
        const glm::vec2 UVS[]
        {
            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(1, 1),
            glm::vec2(0, 1)
        };
        unsigned int TRIANGLE_ELEMENT_COUNT = 6u;
        const unsigned int TRIANGLE_ELEMENTS[]
        {
            0u, 1u, 2u, 0u, 2u, 3u
        };

        _pShader = &GetRendering().CreateShader(
            VERTEX_SHADER_SOURCE,
            FRAGMENT_SHADER_SOURCE
        );

        _pShader->SetInt(SAMPLER_KEY, SAMPLER_UNIT);

        _pMesh = &GetRendering().CreateMesh(
            VERTEX_COUNT,
            VERTICES,
            nullptr,
            nullptr,
            nullptr,
            UVS,
            nullptr,
            nullptr,
            TRIANGLE_ELEMENT_COUNT,
            TRIANGLE_ELEMENTS
        );

        _pMaterial = &GetRendering().CreateMaterial();
        _pMaterial->SetShader(_pShader);
        _pMaterial->SetTextureBinding(*pRenderTexture, SAMPLER_UNIT);

        auto& modelRendererComponent = *gameObject().GetComponent<PREModelRendererComponent>();
        modelRendererComponent.SetMesh(_pMesh);
        modelRendererComponent.SetMaterial(_pMaterial);
    }

    void OnDestroy() override
    {
        GetRendering().DestroyShader(*_pShader);
        GetRendering().DestroyMesh(*_pMesh);
        GetRendering().DestroyMaterial(*_pMaterial);
    }

private:
    PREShader* _pShader = nullptr;
    PREMesh* _pMesh = nullptr;
    PREMaterial* _pMaterial = nullptr;
};

class BufferDisplayScreenTemplate : public PREGameObjectTemplate
{
protected:
    void OnInstantiateTemplate() override
    {
        AddPREComponent<PREModelRendererComponent>();
        AddPREComponent<BufferDisplayScreenComponent>();
        GetPREComponent<PRETransformComponent>()->SetScale(glm::vec3(4.0f / 3.0f, 1.0f, 1.0f));
    };
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
};

class FloorTemplate : public PREGameObjectTemplate
{
protected:
    void OnInstantiateTemplate() override
    {
        AddPREComponent<PREModelRendererComponent>();
        AddPREComponent<FloorModelComponent>();
    }
};

class LightCubeTemplate : public PREGameObjectTemplate
{
protected:
    void OnInstantiateTemplate() override
    {
        AddPREComponent<PREModelRendererComponent>();
        AddPREComponent<LightCubeComponent>();
    }
};

class AmbientLightTemplate : public PREGameObjectTemplate
{
protected:
    void OnInstantiateTemplate() override
    {
        AddPREComponent<PREAmbientLightComponent>();
    }
};

class PointLightTemplate : public PREGameObjectTemplate
{
protected:
    void OnInstantiateTemplate() override
    {
        AddPREComponent<PREPointLightComponent>();
    }
};

class SpotLightTemplate : public PREGameObjectTemplate
{
protected:
    void OnInstantiateTemplate() override
    {
        auto& spotLightComponent = *AddPREComponent<PRESpotLightComponent>();
        spotLightComponent.SetAttentuationLinear(0.0f);
        spotLightComponent.SetAttentuationQuadratic(0.15f);
        spotLightComponent.SetColor(glm::vec3(0.0f));
        spotLightComponent.SetTag(1);
        AddPREComponent<PREModelRendererComponent>();
    }
};

class DirectionalLightTemplate : public PREGameObjectTemplate
{
    void OnInstantiateTemplate() override
    {
        auto& directionalLightComponent = *AddPREComponent<PREDirectionalLightComponent>();
        directionalLightComponent.SetColor(glm::vec3(1.0f / 4.0f));
    }
};

class BufferDisplayCameraTemplate : public PREGameObjectTemplate
{
protected:
    void OnInstantiateTemplate() override
    {
        AddPREComponent<PRECameraComponent>();
    }
};

class CameraTemplate : public PREGameObjectTemplate
{
protected:
    void OnInstantiateTemplate() override
    {
        AddPREComponent<PRECameraComponent>();
        auto& flyCamControllerComponent = *AddPREComponent<FlyCamControllerComponent>();

        auto& cameraControllerComponent = *AddPREComponent<CameraControllerComponent>();
        cameraControllerComponent.skyBoxRightPath = "/skyboxes/Night MoonBurst/Right+X.png";
        cameraControllerComponent.skyBoxLeftPath = "/skyboxes/Night MoonBurst/Left-X.png";
        cameraControllerComponent.skyBoxTopPath = "/skyboxes/Night MoonBurst/Top+Y.png";
        cameraControllerComponent.skyBoxBottomPath = "/skyboxes/Night MoonBurst/Bottom-Y.png";
        cameraControllerComponent.skyBoxFrontPath = "/skyboxes/Night MoonBurst/Front+Z.png";
        cameraControllerComponent.skyBoxBackPath = "/skyboxes/Night MoonBurst/Back-Z.png";

        auto pTransformComponent = GetPREComponent<PRETransformComponent>();
        pTransformComponent->SetPosition(glm::vec3(0.0f, 1.0f, 2.5f));

        SpotLightTemplate spotLightTemplate;
        auto& spotLight = Instantiate(spotLightTemplate);
        auto pSpotLightTransform = spotLight.GetComponent<PRETransformComponent>();
        pSpotLightTransform->SetParent(pTransformComponent, false);
        pSpotLightTransform->SetLocalPosition(glm::vec3(0.15f, -0.15f, -0.15f));
        flyCamControllerComponent._pSpotLightComponent = spotLight.GetComponent<PRESpotLightComponent>();
    }
};

static PRERenderTexture* pCameraARenderTexture;
static PRERenderTexture* pCameraBRenderTexture;

void OnInitialize(PREApplicationContext& applicationContext)
{
    std::cout << "ON INITIALIZE" << std::endl;
    applicationContext.time.SetFrameLimit(60);

    BufferDisplayCameraTemplate bufferDisplayCameraTemplate;
    BufferDisplayScreenTemplate bufferDisplayScreenTemplate;

    pCameraARenderTexture = &applicationContext.rendering.CreateRenderTexture(1, 1024, 768 / 2);
    pCameraBRenderTexture = &applicationContext.rendering.CreateRenderTexture(1, 1024, 768 / 2);

    auto& bufferCamera = applicationContext.world.Instantiate(bufferDisplayCameraTemplate);
    auto& bufferCameraComponent = *bufferCamera.GetComponent<PRECameraComponent>();
    bufferCameraComponent.SetRenderTexture(&applicationContext.rendering.GetScreenRenderTexture());
    bufferCameraComponent.SetAspectRatio(4.0f / 3.0f);
    bufferCameraComponent.SetSize(1.0f);
    bufferCameraComponent.SetNearClippingPlane(0.0f);
    bufferCameraComponent.SetKind(PRECameraComponent::Kind::ORTHOGRAPHIC);
    bufferCameraComponent.SetTag(0); // technically not necessary; default = 0

    auto& bufferDisplayScreenA = applicationContext.world.Instantiate(bufferDisplayScreenTemplate);
    auto& bufferDisplayScreenAComponent = *bufferDisplayScreenA.GetComponent<BufferDisplayScreenComponent>();
    bufferDisplayScreenAComponent.pRenderTexture = pCameraARenderTexture;
    bufferDisplayScreenA.GetComponent<PREModelRendererComponent>()->SetTag(0);

    auto& bufferDisplayScreenB = applicationContext.world.Instantiate(bufferDisplayScreenTemplate);
    bufferDisplayScreenB.GetComponent<PRETransformComponent>()->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    auto& bufferDisplayScreenBComponent = *bufferDisplayScreenB.GetComponent<BufferDisplayScreenComponent>();
    bufferDisplayScreenBComponent.pRenderTexture = pCameraBRenderTexture;
    bufferDisplayScreenB.GetComponent<PREModelRendererComponent>()->SetTag(0);

    // borrow this; require at least one light for anything to render
    AmbientLightTemplate nullLightTemplate;

    auto &nullLightComponent = *applicationContext.world
        .Instantiate(nullLightTemplate)
        .GetComponent<PREAmbientLightComponent>();
    nullLightComponent.SetTag(0); // technically not necessary; default = 0

    VampireTemplate capoeiraTemplate, thrillerTemplate;
    capoeiraTemplate.animationPath = "/animations/mixamo/Capoeira.dae";
    thrillerTemplate.animationPath = "/animations/mixamo/Running.dae";

    FloorTemplate floorTemplate;

    CameraTemplate cameraTemplate;

    AmbientLightTemplate ambientLightTemplate;
    PointLightTemplate pointLightTemplate;
    DirectionalLightTemplate directionalLightTemplate;

    LightCubeTemplate lightCubeTemplate;

    auto& sceneRoot = applicationContext.world.Instantiate();
    auto& sceneRootTransform = *sceneRoot.GetComponent<PRETransformComponent>();

    auto& cameraA = applicationContext.world.Instantiate(cameraTemplate);
    auto& cameraAComponent = *cameraA.GetComponent<PRECameraComponent>();
    cameraAComponent.SetRenderTexture(pCameraARenderTexture);
    cameraAComponent.SetTag(1);
    auto& cameraATransform = *cameraA.GetComponent<PRETransformComponent>();
    cameraATransform.SetParent(&sceneRootTransform, true);

    auto& cameraB = applicationContext.world.Instantiate(cameraTemplate);
    auto& cameraBComponent = *cameraB.GetComponent<PRECameraComponent>();
    cameraBComponent.SetRenderTexture(pCameraBRenderTexture);
    cameraBComponent.SetTag(1);
    auto& cameraBTransform = *cameraB.GetComponent<PRETransformComponent>();
    cameraBTransform.SetParent(&sceneRootTransform, true);
    cameraB.GetComponent<FlyCamControllerComponent>()->enabled = false;

    auto& vampireA = applicationContext.world.Instantiate(capoeiraTemplate);
    auto& vampireATransform = *vampireA.GetComponent<PRETransformComponent>();
    vampireATransform.SetPosition(
        vampireATransform.GetPosition() + glm::vec3(-1, 0, 0)
    );
    vampireATransform.SetParent(&sceneRootTransform, true);
    auto pVampireAModelRendererComponent = vampireA.GetComponent<PREModelRendererComponent>();
    pVampireAModelRendererComponent->SetTag(1);

    for (auto i = 0; i < 1; ++i)
    {
        auto& vampireB = applicationContext.world.Instantiate(thrillerTemplate);
        auto& vampireBTransform = *vampireB.GetComponent<PRETransformComponent>();
        vampireBTransform.SetPosition(
            vampireBTransform.GetPosition() + glm::vec3(1 + i, 0, 0)
        );
        vampireBTransform.SetParent(&sceneRootTransform, true);
        auto pVampireBModelRendererComponent = vampireB.GetComponent<PREModelRendererComponent>();
        pVampireBModelRendererComponent->SetTag(1);
    }

    auto& floor = applicationContext.world.Instantiate(floorTemplate);
    auto pFloorTransform = floor.GetComponent<PRETransformComponent>();
    pFloorTransform->SetEuler(glm::vec3(-90.0f, 0.0f, 0.0f));
    pFloorTransform->SetScale(glm::vec3(8.0f));
    auto pFloorModelRendererComponent = floor.GetComponent<PREModelRendererComponent>();
    pFloorModelRendererComponent->SetTag(1);

    sceneRootTransform.SetEuler(glm::vec3(0, 180, 0));

    glm::vec3 lightPositions[] { glm::vec3(-2.5f, 2, 0), glm::vec3(2.5f, 2, 0), glm::vec3(0, 2, -2.5f), glm::vec3(0, 2, 2.5f) };
    glm::vec3 lightColors[]{ glm::vec3(1, 0, 0), glm::vec3(1, 0, 1), glm::vec3(0, 1, 1), glm::vec3(0, 1, 0) };
    float linearLightLuminosities[]{ 0.35f, 0.07f, 0.1f, 0.1f };
    float quadraticLightLuminosities[]{ 0.05f, 0.07f, 0.14f, 0.14f };

    auto& ambientLight = applicationContext.world.Instantiate(ambientLightTemplate);
    auto& ambientLightComponent = *ambientLight.GetComponent<PREAmbientLightComponent>();
    ambientLightComponent.SetAttentuationLinear(0.2f);
    ambientLightComponent.SetAttentuationQuadratic(0.45f);
    ambientLightComponent.SetColor(glm::vec3(0.5f));
    ambientLightComponent.SetTag(1);

    auto& directionalLight = applicationContext.world.Instantiate(directionalLightTemplate);
    auto& directionalLightTransform = *directionalLight.GetComponent<PRETransformComponent>();
    directionalLightTransform.SetEuler(glm::vec3(-35.f, 0.0f, 0.0f));
    directionalLight.GetComponent<PREDirectionalLightComponent>()->SetTag(1);

    for (auto i = 0; i < 4; ++i)
    {
        auto& light = applicationContext.world.Instantiate(pointLightTemplate);
        auto& lightTransform = *light.GetComponent<PRETransformComponent>();
        lightTransform.SetPosition(lightPositions[i]);
        lightTransform.SetParent(&sceneRootTransform, false);
        auto& lightComponent = *light.GetComponent<PREPointLightComponent>();
        lightComponent.SetAttentuationLinear(linearLightLuminosities[i]);
        lightComponent.SetAttentuationQuadratic(quadraticLightLuminosities[i]);
        lightComponent.SetColor(lightColors[i]);
        lightComponent.SetTag(1);
        auto& lightCube = applicationContext.world.Instantiate(lightCubeTemplate);
        auto& lightCubeTransform = *lightCube.GetComponent<PRETransformComponent>();
        lightCubeTransform.SetParent(&lightTransform, false);
        auto& lightCubeComponent = *lightCube.GetComponent<LightCubeComponent>();
        lightCubeComponent.color = lightColors[i];
        auto plightModelRendererComponent = lightCube.GetComponent<PREModelRendererComponent>();
        plightModelRendererComponent->SetTag(1);
    }
}

void OnShutdown(PREApplicationContext& applicationContext)
{
    applicationContext.rendering.DestroyRenderTexture(*pCameraARenderTexture);
    applicationContext.rendering.DestroyRenderTexture(*pCameraBRenderTexture);
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
                    "PRE v0.36 DEV",
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