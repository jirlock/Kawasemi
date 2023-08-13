#include "Component.h"
#include "Actor.h"

Component::Component(Actor* actor, int updateOrder)
	: mOwner(actor)
	, mWindow(mOwner->GetWindow())
{
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
