#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "MeshComponent.h"
#include "LightComponent.h"



Actor::Actor(Game* game, std::string name)
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
    mGame->RemoveActor(this);

    while (!mComponents.empty())
    {
        delete mComponents.back();
    }
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
    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
		mComponents.erase(iter);
}

void Actor::AddPointlightComponent(PointlightComponent* component)
{
    mPointlights.push_back(component);
}

void Actor::RemovePointlightComponent(PointlightComponent* component)
{
    auto iter = std::find(mPointlights.begin(), mPointlights.end(), component);
    if (iter != mPointlights.end())
        mPointlights.erase(iter);
}

void Actor::AddMeshComponent(MeshComponent* component)
{
    mMeshes.push_back(component);
}

void Actor::RemoveMeshComponent(MeshComponent* component)
{
    auto iter = std::find(mMeshes.begin(), mMeshes.end(), component);
    if (iter != mMeshes.end())
        mMeshes.erase(iter);
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
