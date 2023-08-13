#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Game
{
public:
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

	void SetCamera(class CameraActor* camera) { mCamera = camera; }

private:
	bool InitializeWindow();
	void LoadScene();
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void ConfigureFrameSize();

	class GLFWwindow* mWindow;
	bool mIsRunning;
	bool mUpdatingActors;

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class MeshComponent*> mMeshComponents;
	class CameraActor* mCamera;
	class Renderer* mRenderer;
	class Gui* mGui;

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
