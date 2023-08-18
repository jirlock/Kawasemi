#pragma once

#include "Actor.h"

class CameraActor : public Actor
{
public:
	CameraActor(class Game* game, const char* name, glm::vec3 position, glm::vec3 rotation, float fov, float near, float far, float moveSpeed, float mouseSpeed);
	void ActorInput() override;
	void UpdateActor(float deltaTime) override;
	void UpdateDirection(float deltaTime);
	void UpdatePosition(float deltaTime);
	void UpdateMatrices() override;

    float GetMoveSpeed() { return mMoveSpeed; }
    float GetMouseSpeed() { return mMouseSpeed; }
    float GetFov() { return mFov; }
    float GetNear() { return mNear; }
    float GetFar() { return mFar; }
	glm::mat4 GetVPMatrix() { return mVPMatrix; }
    glm::vec3 GetDirection() { return mDirection; }

    void SetMoveSpeed(float speed) { mMoveSpeed = speed; }
    void SetMouseSpeed(float speed) { mMouseSpeed = speed; }
    void SetFov(float fov) { mFov = fov; }
    void SetNear(float near) { mNear = near; }
    void SetFar(float far) { mFar = far; }

private:
	float mFov;
	float mNear, mFar;
	float mMoveSpeed, mMouseSpeed;

	bool mW, mS, mD, mA, mE, mQ;
	bool mIsOnCamera;
	double mMouseXPos, mMouseYPos;
	double mStoredXPos, mStoredYPos;
	glm::vec3 mDirection;
	glm::vec3 mRight;
	glm::vec3 mUp;
	glm::mat4 mProjectionMatrix;
	glm::mat4 mViewMatrix;
	glm::mat4 mVPMatrix;
};
