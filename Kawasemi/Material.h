#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Material
{
public:
	Material(std::string name, class Texture* texture = nullptr, glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f), bool useTexture = false);
	class Texture* GetTexture() { return mTexture; }
	GLuint GetTextureID();
	bool UseTexture() { return mUseTexture; }
    std::string GetName() { return mName; }
	glm::vec3 GetColor() { return mColor; }


    void SetColor(glm::vec3 color) { mColor = color; }

private:
	std::string mName;
	class Texture* mTexture;
	glm::vec3 mColor;
    bool mUseTexture;
};
