#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Component
{ 
public:
	Component(class Actor* owner, int updateOrder=100);
	virtual ~Component();
	virtual void Update(float deltaTime);
	virtual void ProcessInput();

	int GetUpdateOrder() const { return mUpdateOrder; }
	class Actor* GetOwner() { return mOwner; }

protected:
	class Actor* mOwner;
	int mUpdateOrder;
	GLFWwindow* mWindow;
};
