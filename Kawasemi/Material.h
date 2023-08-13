#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Material
{
public:
	Material(const char* name, class Texture* texture = nullptr, glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f), bool useTexture = false);
	bool UseTexture() { return mUseTexture; }
	class Texture* GetTexture() { return mTexture; }
	GLuint GetTextureID();
	glm::vec3 GetColor() { return mColor; }

private:
	const char* mName;
	class Texture* mTexture;
	glm::vec3 mColor;
    bool mUseTexture;
};
