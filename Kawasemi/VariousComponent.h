#pragma once

#include "Component.h"

#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

class ChangeColorComponent : public Component
{
public:
    ChangeColorComponent(class Actor* actor, class Material* material, int updateOrder = 100);
    void Update(float deltaTime) override;
private:
    glm::vec3 CreateRandomColor();

    class Material* mMaterial;
};
