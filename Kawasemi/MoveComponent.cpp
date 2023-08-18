#include "MoveComponent.h"
#include "Actor.h"
#include "MeshComponent.h"

MoveComponent::MoveComponent(Actor* actor, MeshComponent* comp, int updateOrder)
    : Component(actor, Component::TNormal, updateOrder)
    , mMesh(comp)
{

}

void MoveComponent::Update(float deltaTime)
{
    glm::vec3 tmpRot = mMesh->GetRotation();
    tmpRot.z += deltaTime * glm::radians(90.0f);
    mMesh->SetRotation(tmpRot);
}