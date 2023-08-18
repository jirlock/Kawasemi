#pragma once

#include "Component.h"

class MoveComponent : public Component
{
public:
    MoveComponent(class Actor* actor, class MeshComponent* comp, int updateOrder = 100);
    void Update(float deltaTime) override;

private:
    class MeshComponent* mMesh;
};
