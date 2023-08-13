#include "Actor.h"
#include "Game.h"
#include "Component.h"



Actor::Actor(Game* game, const char* name)
	: mName(name)
	, mState(EActive)
	, mPosition(glm::vec3(0.0f, 0.0f, 0.0f))
	, mRotation(glm::vec3(0.0f, 0.0f, 0.0f))
	, mScale(glm::vec3(1.0f, 1.0f, 1.0f))
	, mGame(game)
	, mWindow(game->GetWindow())
{
	mGame->AddActor(this);
	UpdateMatrices();
}

Actor::~Actor()
{

}

void Actor::ProcessInput()
{
	if (mState == EActive)
	{
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}

		ActorInput();
	}
}

void Actor::ActorInput()
{

}

void Actor::Update(float deltaTime)
{
	UpdateComponents(deltaTime);
	UpdateActor(deltaTime);
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto component : mComponents)
	{
		component->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
	UpdateMatrices();
}

void Actor::AddComponent(Component* component)
{
	int mOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); iter++)
	{
		if (mOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}


void Actor::UpdateMatrices()
{
	mScaleMatrix = glm::scale(glm::mat4(1.0f), mScale);
	glm::mat4 tmp = glm::rotate(glm::mat4(1.0f), mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	tmp = glm::rotate(tmp, mRotation.y, glm::vec3(0.0f, 0.0f, 1.0f));
	mRotationMatrix = glm::rotate(tmp, mRotation.z, glm::vec3(0.0f, 1.0f, 0.0f));
	mTranslationMatrix = glm::translate(glm::mat4(1.0f), mPosition);
	mModelMatrix = mTranslationMatrix * mRotationMatrix * mScaleMatrix;
}
