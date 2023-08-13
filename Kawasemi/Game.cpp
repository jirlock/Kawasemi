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

Game::Game()
    : mIsRunning(true)
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
    new CameraActor(this, "Camera");
    mRenderer = new Renderer(this);
    mRenderer->Initialize();

    Actor* AKawasemi = new Actor(this, "Kawasemi");
    Mesh* MeshKawasemi = new Mesh("Meshes/stanfordBunny.obj", true);
    Texture* TexKawasemi = new Texture("Textures/TexKawasemi.bmp");
    Material* MaterialKawasemi = new Material("MatKawasemi", TexKawasemi, glm::vec3(1.0f, 0.5f, 0.5f));
    MeshComponent* MeshCompKawasemi = new MeshComponent(AKawasemi, MeshKawasemi, MaterialKawasemi);

    Actor* AFloor = new Actor(this, "Floor");
    Mesh* MeshFloor = new Mesh("Meshes/Floor.obj", false);
    Texture* TexFloor = new Texture("Textures/snow.bmp");
    Material* MaterialFloor = new Material("MatFloor", TexFloor, glm::vec3(0.5f, 0.5f, 0.5f));
    MeshComponent* MeshCompFloor = new MeshComponent(AFloor, MeshFloor, MaterialFloor);

    Actor* APointlight0 = new Actor(this, "Pointlight0");
    PointlightComponent* PointComp0 = new PointlightComponent(APointlight0, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    APointlight0->SetPosition(glm::vec3(-4.0f, 3.0f, 3.0f));

    Actor* APointlight1 = new Actor(this, "Pointlight1");
    PointlightComponent* PointComp1 = new PointlightComponent(APointlight1, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    APointlight1->SetPosition(glm::vec3(3.0f, 3.0f, 5.0f));

    Actor* APointlight2 = new Actor(this, "Pointlight2");
    PointlightComponent* PointComp2 = new PointlightComponent(APointlight2, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    APointlight2->SetPosition(glm::vec3(0.0f, 4.0f, -4.0f));

    //Need to Compile Shaders After Loading the Scene.
    mRenderer->CompileShaders();
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
}

void Game::GenerateOutput()
{
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

