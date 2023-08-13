#include "LightComponent.h"
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"

PointlightComponent::PointlightComponent(Actor* actor, float power, glm::vec3 color, int updateOrder)
    : Component(actor, updateOrder)
    , mPower(power)
    , mPosition(glm::vec3(0.0f, 0.0f, 0.0f))
    , mColor(color)
{
    mOwner->GetGame()->GetRenderer()->AddPointlight(this);
}

glm::vec3 PointlightComponent::GetAbsPosition()
{
    return mOwner->GetPosition() + mPosition;
}

