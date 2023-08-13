#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mesh
{
public:
	Mesh(const char* path, bool smooth = true);
	GLuint GetVertexbuffer() { return mVertexbuffer; }
	GLuint GetNormalbuffer() { return mNormalbuffer; }
	GLuint GetUvbuffer() { return mUvbuffer; }
	GLuint GetNumOfVertices() { return mNumOfVertices; }

private:
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec2> mUvs;

	GLuint mVertexbuffer;
	GLuint mNormalbuffer;
	GLuint mUvbuffer;
	GLuint mNumOfVertices;
};
