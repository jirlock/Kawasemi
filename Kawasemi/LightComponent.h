#pragma once

#include "Component.h"

class PointlightComponent : public Component
{
public:
    PointlightComponent(class Actor* onwer, float power, glm::vec3 color, int updateOrder = 100);

    float GetPower() { return mPower; }
    glm::vec3 GetAbsPosition();
    glm::vec3 GetRltPosition() { return mPosition; }
    glm::vec3 GetColor() { return mColor; }

private:
    float mPower;
    glm::vec3 mPosition;
    glm::vec3 mColor;
};
