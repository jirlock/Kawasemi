#include "VariousComponent.h"
#include "Actor.h"
#include "Material.h"

ChangeColorComponent::ChangeColorComponent(Actor* actor, Material* material, int updateOrder)
    : Component(actor, Component::TNormal, updateOrder)
    , mMaterial(material)
{
    mMaterial->SetColor(CreateRandomColor());
}

void ChangeColorComponent::Update(float deltaTime)
{
    int tmp = std::rand() % 3;
    int tmp2 = std::rand() % 2;
    glm::vec3 tmpCol = mMaterial->GetColor();
    tmpCol[tmp] = std::clamp(float(tmpCol[tmp] + (std::pow(-1, tmp2)) * 0.01f), 0.0f, 1.0f);
    mMaterial->SetColor(tmpCol);
}

glm::vec3 ChangeColorComponent::CreateRandomColor()
{
    /*
    glm::vec3 tmpCol;
    tmpCol.x = glm::linearRand(0.0f, 1.0f);
    tmpCol.y = glm::linearRand(0.0f, 1.0f);
    tmpCol.z = glm::linearRand(0.0f, 1.0f);
    return tmpCol;
    */
    return glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f));
}


