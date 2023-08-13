#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Texture
{
public:
	Texture(const char* path);
	GLuint GetTextureID() { return mTextureID; }

private:
	GLuint mTextureID;

};
