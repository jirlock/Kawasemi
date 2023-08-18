#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};
	Actor(class Game* game, std::string name);
	virtual ~Actor();

	void ProcessInput();
	virtual void ActorInput();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
	void AddMeshComponent(class MeshComponent* component);
	void RemoveMeshComponent(class MeshComponent* component);
	void AddPointlightComponent(class PointlightComponent* component);
	void RemovePointlightComponent(class PointlightComponent* component);
    /*
	void AddSpotlightComponent(class SpotlightComponent* component);
	void RemoveSpotlightComponent(class SpotlightComponent* component);
	*/

	class Game* GetGame() { return mGame; }
	std::string GetName() { return mName; }
	State GetState() const { return mState; }
	GLFWwindow* GetWindow() { return mWindow; }
	glm::vec3 GetPosition() const { return mPosition; }
	glm::vec3 GetRotation() const { return mRotation; }
	glm::vec3 GetScale() const { return mScale; }
	glm::mat4 GetScaleMatrix() { return mScaleMatrix; }
	glm::mat4 GetRotationMatrix() { return mRotationMatrix; }
	glm::mat4 GetTranslationMatrix() { return mTranslationMatrix; }
	glm::mat4 GetModelMatrix() { return mModelMatrix; }
	//glm::vec3 GetDirection();
	//glm::mat4 GetVPMatrix();
    std::vector<Component*> GetComponents() { return mComponents; }
    std::vector<MeshComponent*> GetMeshComponents() { return mMeshes; }
    std::vector<PointlightComponent*> GetPointlightComponents() { return mPointlights; }

	void SetPosition(glm::vec3 position) { mPosition = position; }
	void SetRotation(glm::vec3 rotation) { mRotation = rotation; }
	void SetScale(glm::vec3 scale) { mScale = scale; }

    void MoveActor(glm::vec3 position) { mPosition += position; }
	
protected:
	virtual void UpdateMatrices();

	std::string mName;
	State mState;
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;
	glm::mat4 mScaleMatrix;
	glm::mat4 mRotationMatrix;
	glm::mat4 mTranslationMatrix;
	glm::mat4 mModelMatrix;

	class Game* mGame;
	GLFWwindow* mWindow;
	std::vector<class Component*> mComponents;
    std::vector<class PointlightComponent*> mPointlights;
    std::vector<class MeshComponent*> mMeshes;
};
