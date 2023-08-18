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
	Texture(std::string path);
	GLuint GetTextureID() { return mTextureID; }
    std::string GetPath() { return mPath; }

private:
    std::string mPath;
	GLuint mTextureID;

};
