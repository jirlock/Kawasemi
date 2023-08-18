#pragma once

#include "Component.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* actor, class Mesh* mesh = nullptr, class Material* material = nullptr, int updateOrder=100);
	void Update(float deltaTime) override;

	class Mesh* GetMesh() { return mMesh; }
	class Material* GetMaterial() { return mMaterial; }
    glm::mat4 GetRotationMatrix();
	glm::mat4 GetModelMatrix();
    glm::vec3 GetPosition() { return mPosition; }
    glm::vec3 GetRotation() { return mRotation; }
    glm::vec3 GetScale() { return mScale; }

	void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetMaterial(class Material* material) { mMaterial = material; }
    void SetPosition(glm::vec3 position) { mPosition = position; }
    void SetRotation(glm::vec3 rotation) { mRotation = rotation; }
    void SetScale(glm::vec3 scale) { mScale = scale; }

private:
	void UpdateMatrices();

	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;
	glm::mat4 mScaleMatrix;
	glm::mat4 mRotationMatrix;
	glm::mat4 mTranslationMatrix;
	glm::mat4 mModelMatrix;
	class Mesh* mMesh;
	class Material* mMaterial;
};
