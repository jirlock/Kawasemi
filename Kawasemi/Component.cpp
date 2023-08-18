#include "Component.h"
#include "Actor.h"

Component::Component(Actor* actor, Component::CompType type, int updateOrder)
    : mOwner(actor)
    , mWindow(mOwner->GetWindow())
    , mType(type)
{
    mOwner->AddComponent(this);
}

Component::~Component()
{
}

void Component::Update(float deltaTime)
{

}

void Component::ProcessInput()
{

}
