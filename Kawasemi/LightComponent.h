#pragma once

#include "Component.h"

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class PointlightComponent : public Component
{
public:
    PointlightComponent(class Actor* onwer, float power, glm::vec3 position, glm::vec3 color, int updateOrder = 100);
    void Update(float deltaTime) override;

    std::vector<glm::mat4> GetShadowTransforms() { return mShadowTransforms; }
    float GetPower() { return mPower; }
    glm::vec3 GetAbsPosition();
    glm::vec3 GetRltPosition() { return mPosition; }
    glm::vec3 GetColor() { return mColor; }
    float GetNear() { return mNear; }
    float GetFar() { return mFar; }

    void SetPower(float power) { mPower = power; }
    void SetPosition(glm::vec3 position) { mPosition = position; }
    void SetColor(glm::vec3 color) { mColor = color; }
    void SetNear(float near) { mNear = near; }
    void SetFar(float far) { mFar = far; }

private:
    void UpdateShadowTransforms();

    float mPower;
    glm::vec3 mPosition;
    glm::vec3 mColor;
    float mNear;
    float mFar;
    std::vector<glm::mat4> mShadowTransforms;
};
