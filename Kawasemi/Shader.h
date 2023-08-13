#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const char* vspath, const char* fspath, const char* gspath = nullptr, std::vector<std::string> rep_from = {}, std::vector<std::string> rep_to = {});
	void Use() { glUseProgram(mProgramID); }
	GLuint GetProgramID() { return mProgramID; }
	GLuint GetAttribLocation(const char* name) { return glGetAttribLocation(mProgramID, name); }
	GLuint GetUniformLocation(const char* name) { return glGetUniformLocation(mProgramID, name); }
	void ConfigureAttrib(const char* name, GLuint buffer, GLuint n);
	void ConfigureTexture(const char* name, GLuint texture, GLuint unit);
	void ConfigureCubeTexture(const char* name, GLuint texture, GLuint unit);
	void ConfigureTextureArray(const char* name, GLuint texture, GLuint unit);
	void ConfigureCubeArray(const char* name, GLuint texture, GLuint unit);
	void ConfigureInt(const char* name, int a) { glUniform1i(GetUniformLocation(name), a); }
	void ConfigureFloat(const char* name, float a) { glUniform1f(GetUniformLocation(name), a); }
	void ConfigureVec3(const char* name, float* vec) { glUniform3fv(GetUniformLocation(name), 1, vec); }
	void ConfigureMat4(const char* name, float* mat) { glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, mat); }

private:
	GLuint mProgramID;
};
