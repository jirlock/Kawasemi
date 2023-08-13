#include "MeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

MeshComponent::MeshComponent(Actor* actor, Mesh* mesh, Material* material, int updateOrder)
	: Component(actor, updateOrder)
	, mPosition(glm::vec3(0.0f, 0.0f, 0.0f))
	, mRotation(glm::vec3(0.0f, 0.0f, 0.0f))
	, mScale(glm::vec3(1.0f, 1.0f, 1.0f))
	, mMesh(mesh)
	, mMaterial(material)
{
	UpdateMatrices();
    mOwner->GetGame()->GetRenderer()->AddMesh(this);
}

void MeshComponent::Update(float deltaTime)
{
	UpdateMatrices();
}

glm::mat4 MeshComponent::GetRotationMatrix()
{
    return mOwner->GetRotationMatrix() * mRotationMatrix;
}

glm::mat4 MeshComponent::GetModelMatrix()
{
	return mOwner->GetModelMatrix() * mModelMatrix;
}

void MeshComponent::UpdateMatrices()
{
	mScaleMatrix = glm::scale(glm::mat4(1.0f), mScale);
	glm::mat4 tmp = glm::rotate(glm::mat4(1.0f), mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	tmp = glm::rotate(tmp, mRotation.y, glm::vec3(0.0f, 0.0f, 1.0f));
	mRotationMatrix = glm::rotate(tmp, mRotation.z, glm::vec3(0.0f, 1.0f, 0.0f));
	mTranslationMatrix = glm::translate(glm::mat4(1.0f), mPosition);
	mModelMatrix = mTranslationMatrix * mRotationMatrix * mScaleMatrix;
}