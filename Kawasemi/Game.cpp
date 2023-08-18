#include "Game.h"
#include "Gui.h"
#include "Actor.h"
#include "CameraActor.h"
#include "Renderer.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Texture.h"
#include "LightComponent.h"
#include "MoveComponent.h"
#include "VariousComponent.h"
#include "Scene.h"


Game::Game()
    : mGameState(EEdit)
    , mIsRunning(true)
    , mUpdatingActors(false)
    , mWindowWidth(1600)
    , mWindowHeight(800)
{
}

bool Game::Initialize()
{
    InitializeWindow();

    LoadScene();

    mGui = new Gui(this, mWindow);
    mGui->Initialize();

    return true;
}


bool Game::InitializeWindow()
{
	if (!glfwInit()) {
			std::cout << "Failed to Initialize GLFW" << std::endl;
			return false;
		}
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "Kawasemi", NULL, NULL);
		glfwMakeContextCurrent(mWindow);

		gladLoadGL();

		glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSetInputMode(mWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        ConfigureFrameSize();
}

void Game::LoadScene()
{
    mRenderer = new Renderer(this);
    mScene = new Scene();

    //Load
    //LoadDefaultScene();
    LoadFromJson("Scene/tmpScene.json");

    //Need to Compile Shaders After Loading the Scene.
    mRenderer->Initialize();
    mRenderer->CompileShaders();
    mRenderer->PrepareShadowFBO();
}

void Game::LoadDefaultScene()
{
    new CameraActor(this, "Camera", glm::vec3(0.0f, 1.7f, -4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(60.0f), 0.1f, 100.0f, 6.0f, 0.1f);

    mRenderer = new Renderer(this);
    mRenderer->Initialize();

    Actor* ARabbit = new Actor(this, "Rabbit");
    Mesh* MeshRabbit = new Mesh("Meshes/stanfordBunny.obj", true);
    Texture* TexSnow = new Texture("Textures/Snow.bmp");
    Material* MaterialRabbit = new Material("MatRabbit", TexSnow, glm::vec3(0.7f, 0.7f, 0.7f));
    MeshComponent* MeshCompRabbit = new MeshComponent(ARabbit, MeshRabbit, MaterialRabbit);

    Actor* AFloor = new Actor(this, "Floor");
    Mesh* MeshFloor = new Mesh("Meshes/Floor.obj", false);
    Texture* TexFloor = new Texture("Textures/snow.bmp");
    Material* MaterialFloor = new Material("MatFloor", TexFloor, glm::vec3(0.5f, 0.5f, 0.5f));
    MeshComponent* MeshCompFloor = new MeshComponent(AFloor, MeshFloor, MaterialFloor);

    Actor* APointlight0 = new Actor(this, "Pointlight0");
    PointlightComponent* PointComp0 = new PointlightComponent(APointlight0, 30.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    APointlight0->SetPosition(glm::vec3(-4.0f, 5.0f, 3.0f));

    Actor* APointlight1 = new Actor(this, "Pointlight1");
    PointlightComponent* PointComp1 = new PointlightComponent(APointlight1, 30.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    APointlight1->SetPosition(glm::vec3(3.0f, 5.0f, 5.0f));

    Actor* APointlight2 = new Actor(this, "Pointlight2");
    PointlightComponent* PointComp2 = new PointlightComponent(APointlight2, 10.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    APointlight2->SetPosition(glm::vec3(0.0f, 4.0f, -4.0f));
}

void Game::LoadFromJson(const char* filepath)
{
    mScene->LoadScene(filepath);
    Scene::SceneStruct sceneStruct = mScene->GetSceneStructure();
    LoadCamera(sceneStruct.camera);

    for (auto actor : sceneStruct.actors)
    {
        LoadActor(actor);
    }
}

void Game::LoadActor(Scene::ActorStruct actorStruct)
{
    std::cout << "Loading Actor: " << actorStruct.name << std::endl;

    Actor* actor = new Actor(this, actorStruct.name);
    actor->SetPosition(actorStruct.position);
    actor->SetRotation(actorStruct.rotation);
    actor->SetScale(actorStruct.scale);
    for (int i = 0; i < actorStruct.meshComponents.size(); i++) {
        Scene::MeshCompStruct meshCompStruct = actorStruct.meshComponents[i];
        Mesh* mesh = new Mesh(meshCompStruct.mesh.path, meshCompStruct.mesh.smooth);
        Texture* texture = new Texture(meshCompStruct.material.texture.path);
        Material* material = new Material(meshCompStruct.material.name, texture, meshCompStruct.material.color);
        MeshComponent* meshcomp = new MeshComponent(actor, mesh, material);
        meshcomp->SetPosition(meshCompStruct.position);
        meshcomp->SetRotation(meshCompStruct.rotation);
        meshcomp->SetScale(meshCompStruct.scale);
    }
    for (int i = 0; i < actorStruct.pointlightComponents.size(); i++) {
        Scene::PointlightCompStruct pointCompStruct = actorStruct.pointlightComponents[i];
        PointlightComponent* pointComp = new PointlightComponent(actor, pointCompStruct.power, pointCompStruct.position, pointCompStruct.color);
    }
}

void Game::LoadCamera(Scene::CameraStruct cameraStruct)
{
    CameraActor* camera = new CameraActor(this, "Camera", cameraStruct.position, cameraStruct.rotation, cameraStruct.fov, cameraStruct.near, cameraStruct.far, cameraStruct.moveSpeed, cameraStruct.mouseSpeed);
}

void Game::SaveScene(const char* filepath)
{
    //Game Scene -> Scene Structure.
    Scene::SceneStruct scene;

    for (int i = 0; i < mActors.size(); i++) {
        if (mActors[i] == mCamera)
        {
            Scene::CameraStruct camera;
            camera.position = mCamera->GetPosition();
            camera.rotation = mCamera->GetRotation();
            camera.fov = mCamera->GetFov();
            camera.near = mCamera->GetNear();
            camera.far = mCamera->GetFar();
            camera.mouseSpeed = mCamera->GetMouseSpeed();
            camera.moveSpeed = mCamera->GetMoveSpeed();
            scene.camera = camera;
        }
        else
        {
            Actor* actor = mActors[i];
            Scene::ActorStruct actorStruct;

            actorStruct = CreateActorStruct(actor);

            scene.actors.push_back(actorStruct);
        }
    }

    //Scene Structure -> Scene Json.
    mScene->SaveScene(filepath, scene);
}

Scene::ActorStruct Game::CreateActorStruct(Actor* actor)
{
    Scene::ActorStruct actorStruct;
    actorStruct.name = actor->GetName();
    actorStruct.position = actor->GetPosition();
    actorStruct.rotation = actor->GetRotation();
    actorStruct.scale = actor->GetScale();

    for (int i = 0; i < actor->GetMeshComponents().size(); i++) {
        MeshComponent* meshcomp = actor->GetMeshComponents()[i];

        Scene::MeshCompStruct meshcompStruct;
        Scene::MeshStruct meshStruct;
        Scene::MaterialStruct materialStruct;
        Scene::TextureStruct textureStruct;

        textureStruct.path = meshcomp->GetMaterial()->GetTexture()->GetPath();
        materialStruct.name = meshcomp->GetMaterial()->GetName();
        materialStruct.texture = textureStruct;
        materialStruct.color = meshcomp->GetMaterial()->GetColor();
        materialStruct.useTexture = meshcomp->GetMaterial()->UseTexture();
        meshStruct.path = meshcomp->GetMesh()->GetPath();
        meshStruct.smooth = meshcomp->GetMesh()->GetSmooth();
        meshcompStruct.mesh = meshStruct;
        meshcompStruct.material = materialStruct;
        meshcompStruct.position = meshcomp->GetPosition();
        meshcompStruct.rotation = meshcomp->GetRotation();
        meshcompStruct.scale = meshcomp->GetScale();

        actorStruct.meshComponents.push_back(meshcompStruct);
    }

    for (int i = 0; i < actor->GetPointlightComponents().size(); i++) {
        PointlightComponent* pointcomp = actor->GetPointlightComponents()[i];

        Scene::PointlightCompStruct pointlightCompStruct;
        pointlightCompStruct.power = pointcomp->GetPower();
        pointlightCompStruct.position = pointcomp->GetRltPosition();
        pointlightCompStruct.color = pointcomp->GetColor();
        pointlightCompStruct.near = pointcomp->GetNear();
        pointlightCompStruct.far = pointcomp->GetFar();

        actorStruct.pointlightComponents.push_back(pointlightCompStruct);
    }

    return actorStruct;
}

void Game::RunLoop()
{
    std::cout << "Running Game Loop" << std::endl;

    mLastTime = glfwGetTime();

    while (mIsRunning)
    {
        mCurrentTime = glfwGetTime();
        mDeltaTime = mCurrentTime - mLastTime;
        mLastTime = mCurrentTime;

        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }

    //SaveScene("Scene/tmpScene.json");
}

void Game::Shutdown()
{

}

void Game::ProcessInput()
{
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(mWindow))
    {
        mIsRunning = false;
    }

    ConfigureFrameSize();

    for (auto actor : mActors)
    {
        actor->ProcessInput();
    }
}

void Game::UpdateGame()
{
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(mDeltaTime);
    }
    mUpdatingActors = false;
}

void Game::GenerateOutput()
{
    mRenderer->DrawShadow();
    mRenderer->Draw();
    mGui->Draw();

	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

void Game::AddActor(Actor* actor)
{
    mActors.push_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    auto iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
        mActors.erase(iter);
}

void Game::ConfigureFrameSize()
{
    glfwGetWindowSize(mWindow, &mWindowWidth, &mWindowHeight);
    mTopbarHeight = 28;
    mTopbarWidth = mWindowWidth;
    mBottombarHeight = 28;
    mBottombarWidth = mWindowWidth;
    mSidebarWidth = std::max(200, mWindowWidth / 5);
    mSidebarHeight = mWindowHeight - mTopbarHeight - mBottombarHeight;
    mViewportWidth = mWindowWidth - mSidebarWidth;
    mViewportHeight = mWindowHeight - mTopbarHeight - mBottombarHeight;
}


void Game::AddRabbit()
{
    std::string name = "ExRabbit";
    name = CheckOverlap(name);
    Actor* ARabbit = new Actor(this, name);
    Mesh* MeshRabbit = new Mesh("Meshes/stanfordBunny.obj", true);
    Texture* TexSnow = new Texture("Textures/Snow.bmp");
    Material* MaterialRabbit = new Material("MatRabbit", TexSnow, glm::vec3(0.7f, 0.7f, 0.7f));
    MeshComponent* MeshCompRabbit = new MeshComponent(ARabbit, MeshRabbit, MaterialRabbit);
}


std::string Game::CheckOverlap(std::string str)
{
    bool isOverlapping = true;
    std::string newstr = str;
    int index = 0;
    while (isOverlapping)
    {
        std::cout << newstr << std::endl;

        for (int i = 0; i < mActors.size(); i++) {
            if (mActors[i]->GetName() == newstr)
            {
                index++;
                newstr = str + std::to_string(index);
                break;
            }
            if (i == mActors.size() - 1)
            {
                isOverlapping = false;
            }
        }
    }

    return newstr;
}
