#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer
{
public:
	Renderer(class Game* game);
	void Initialize();
	void CompileShaders();
	void Draw();
	void AddPointlight(class PointlightComponent* light) { mPointlights.push_back(light); }
	//void RemovePointlight(class PointlightComponent* light);
	void AddMesh(class MeshComponent* mesh) { mMeshes.push_back(mesh); }
	//void RemoveMesh(class MeshComponent* mesh);

private:
	class Game* mGame;
	GLFWwindow* mWindow;
	class CameraActor* mCamera;
	class Shader* mSceneShader;
    GLuint mTmpVertexArray;
	std::vector<class MeshComponent*> mMeshes;
	std::vector<class PointlightComponent*> mPointlights;
	std::vector<class SpotlightComponent*> mSpotlights;

	glm::vec4 mClearColor;
};
