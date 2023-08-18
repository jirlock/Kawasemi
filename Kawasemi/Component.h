#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Component
{ 
public:
    enum CompType
    {
        TMesh,
        TPointlight,
        TNormal
    };
	Component(class Actor* owner, CompType type, int updateOrder=100);
	virtual ~Component();
	virtual void Update(float deltaTime);
	virtual void ProcessInput();

    CompType GetType() { return mType; }
	int GetUpdateOrder() const { return mUpdateOrder; }
	class Actor* GetOwner() { return mOwner; }

protected:
    CompType mType;
	class Actor* mOwner;
	int mUpdateOrder;
	GLFWwindow* mWindow;
};
