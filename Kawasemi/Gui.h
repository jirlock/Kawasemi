#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

class Gui
{
public:
	Gui(class Game* game, class GLFWwindow* window);
	bool Initialize();
	void Draw();

private:
	void SetColor();
	void SetStyle();
	void LoadFont();
	void DrawTopbar();
	void DrawSidebar();
	void DrawBottombar();
	ImVec4 RGBAColor(int r, int g, int b, int a);

	class Game* mGame;
	class GLFWwindow* mWindow;
	ImFont* mFontDefault;
	ImFont* mFontVox20;
	bool mTopbarOpen;
	bool mSidebarOpen;
	bool mBottombarOpen;

    int mActorIndex;

	struct {
		ImVec4 color1;
		ImVec4 color2;
	} mColorScheme;
};
