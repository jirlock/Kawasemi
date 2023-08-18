#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>

#include "Scene.h"

using json = nlohmann::json;

class Game
{
public:

    enum EngineMode
    {
        MEdit,
        MGame,
        MQuit
    };

    enum GameState
    {
        SPlay,
        SPaused,
        SQuit
    };


	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	GLFWwindow* GetWindow() { return mWindow; }
	class CameraActor* GetCamera() { return mCamera; }
    class Renderer* GetRenderer() { return mRenderer; }
    std::vector<class Actor*> GetActors() { return mActors; }

    GameState GetState() { return mGameState; }

	int GetWindowWidth() const { return mWindowWidth; }
	int GetWindowHeight() const { return mWindowHeight; }
	int GetViewportWidth() const { return mViewportWidth; }
	int GetViewportHeight() const { return mViewportHeight; }
	int GetTopbarWidth() const { return mTopbarWidth; }
	int GetTopbarHeight() const { return mTopbarHeight; }
	int GetSidebarWidth() const { return mSidebarWidth; }
	int GetSidebarHeight() const { return mSidebarHeight; }
	int GetBottombarWidth() const { return mBottombarWidth; }
	int GetBottombarHeight() const { return mBottombarHeight; }

    void SetState(GameState state) { mGameState = state; }
	void SetCamera(class CameraActor* camera) { mCamera = camera; }

    void SaveScene(const char* filepath);

    void AddRabbit();


private:
	bool InitializeWindow();
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadScene();
    void LoadDefaultScene();
    void LoadFromJson(const char* filepath);
    void LoadCamera(Scene::CameraStruct cameraStruct);
    void LoadActor(Scene::ActorStruct actorStruct);
    Scene::ActorStruct CreateActorStruct(class Actor* actor);
	void ConfigureFrameSize();
    std::string CheckOverlap(std::string str);

	class GLFWwindow* mWindow;
	bool mIsRunning;
	bool mUpdatingActors;
    GameState mGameState;

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class MeshComponent*> mMeshComponents;
	class CameraActor* mCamera;
	class Renderer* mRenderer;
	class Gui* mGui;
    class Scene* mScene;

	double mDeltaTime, mLastTime, mCurrentTime;
	int mWindowWidth;
	int mWindowHeight;
	int mViewportWidth;
	int mViewportHeight;
	int mTopbarWidth;
	int mTopbarHeight;
	int mBottombarWidth;
	int mBottombarHeight;
	int mSidebarWidth;
	int mSidebarHeight;
	
};
