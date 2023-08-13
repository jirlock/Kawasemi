#include "CameraActor.h"
#include "Game.h"

CameraActor::CameraActor(Game* game, const char* name)
	: Actor(game, name)
	, mMoveSpeed(4.0f)
	, mMouseSpeed(0.1f)
	, mFov(glm::radians(60.0f))
	, mNear(0.5f)
	, mFar(100.0f)
	, mIsOnCamera(false)
	, mW(false)
	, mS(false)
	, mD(false)
	, mA(false)
	, mUp(glm::vec3(0.0f, 1.0f, 0.0f))
{
    mPosition = glm::vec3(1.0f, 3.0f, -3.0f);
	mRotation.x = 0.0f;
	mDirection = glm::vec3(
		sin(mRotation.z) * cos(mRotation.y),
		sin(mRotation.y),
		cos(mRotation.z) * cos(mRotation.y)
	);
	mRight = glm::cross(mDirection, mUp);
	UpdateMatrices();
	mGame->SetCamera(this);
}

void CameraActor::ActorInput()
{
	bool RMBClicked = (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
	if (RMBClicked) {
		if (!mIsOnCamera) {
			mIsOnCamera = true;
			glfwGetCursorPos(mWindow, &mStoredXPos, &mStoredYPos);
			glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPos(mWindow, mGame->GetWindowWidth() / 2, mGame->GetWindowHeight() / 2);
		}

		glfwGetCursorPos(mWindow, &mMouseXPos, &mMouseYPos);
		glfwSetCursorPos(mWindow, mGame->GetWindowWidth() / 2, mGame->GetWindowHeight() / 2);
		mW = glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS;
		mS = glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS;
		mA = glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS;
		mD = glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS;
		mE = glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS;
		mQ = glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_PRESS;
	}
	else {
		if (mIsOnCamera) {
			mIsOnCamera = false;
			glfwSetCursorPos(mWindow, mStoredXPos, mStoredYPos);
			glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}

void CameraActor::UpdateActor(float deltaTime)
{
    if (mIsOnCamera)
    {
        UpdateDirection(deltaTime);
        UpdatePosition(deltaTime);
    }
    UpdateMatrices();
}

void CameraActor::UpdateDirection(float deltaTime)
{
	mRotation.z += deltaTime * mMouseSpeed * float(mGame->GetWindowWidth() / 2 - mMouseXPos);
	mRotation.y += deltaTime * mMouseSpeed * float(mGame->GetWindowHeight() / 2 - mMouseYPos);
	mRotation.y = std::clamp(mRotation.y, -3.14f / 2.0f, 3.14f / 2.0f);
	mDirection = glm::vec3(
		sin(mRotation.z) * cos(mRotation.y),
		sin(mRotation.y),
		cos(mRotation.z) * cos(mRotation.y)
	);
	mRight = glm::cross(mDirection, mUp);
}

void CameraActor::UpdatePosition(float deltaTime)
{
	if (mW)
		mPosition += mDirection * deltaTime * mMoveSpeed;
	if (mS)
		mPosition -= mDirection * deltaTime * mMoveSpeed;
	if (mD)
		mPosition += mRight * deltaTime * mMoveSpeed;
	if (mA)
		mPosition -= mRight * deltaTime * mMoveSpeed;
	if (mE)
		mPosition += mUp * deltaTime * mMoveSpeed;
	if (mQ)
		mPosition -= mUp * deltaTime * mMoveSpeed;
}

void CameraActor::UpdateMatrices()
{
	mViewMatrix = glm::lookAt(mPosition, mPosition + mDirection, mUp);
	mProjectionMatrix = glm::perspective(mFov, float(mGame->GetViewportWidth()) / float(mGame->GetViewportHeight()), mNear, mFar);
	mVPMatrix = mProjectionMatrix * mViewMatrix;
}
