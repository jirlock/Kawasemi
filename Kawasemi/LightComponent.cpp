#include "LightComponent.h"
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"

PointlightComponent::PointlightComponent(Actor* actor, float power, glm::vec3 position, glm::vec3 color, int updateOrder)
    : Component(actor, Component::TPointlight, updateOrder)
    , mPower(power)
    , mPosition(position)
    , mColor(color)
    , mNear(0.5f)
    , mFar(50.0f)
{
    mOwner->AddPointlightComponent(this);
    mOwner->GetGame()->GetRenderer()->AddPointlight(this);
    UpdateShadowTransforms();
}

void PointlightComponent::Update(float deltaTime)
{
    UpdateShadowTransforms();
}

glm::vec3 PointlightComponent::GetAbsPosition()
{
    return mOwner->GetPosition() + mPosition;
}

void PointlightComponent::UpdateShadowTransforms()
{
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, mNear, mFar);

    if (mShadowTransforms.size() != 0)
        mShadowTransforms.clear();

    glm::vec3 tmpPos = GetAbsPosition();
    mShadowTransforms.push_back(shadowProj * glm::lookAt(tmpPos, tmpPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    mShadowTransforms.push_back(shadowProj * glm::lookAt(tmpPos, tmpPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    mShadowTransforms.push_back(shadowProj * glm::lookAt(tmpPos, tmpPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    mShadowTransforms.push_back(shadowProj * glm::lookAt(tmpPos, tmpPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    mShadowTransforms.push_back(shadowProj * glm::lookAt(tmpPos, tmpPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    mShadowTransforms.push_back(shadowProj * glm::lookAt(tmpPos, tmpPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
}

