
#include "Gui.h"
#include "Game.h"
#include "Actor.h"
#include "CameraActor.h"

Gui::Gui(Game* game, GLFWwindow* window)
	: mGame(game)
	, mWindow(window)
	, mTopbarOpen(true)
{
}

bool Gui::Initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	SetColor();
	SetStyle();
	LoadFont();

	return true;
}


void Gui::SetColor()
{
	mColorScheme.color1 = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	mColorScheme.color2 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Gui::SetStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowMinSize = ImVec2(1.0f, 1.0f);
	style.WindowBorderSize = 0.0f;
	style.WindowRounding = 0.0f;
}

void Gui::LoadFont()
{
	ImGuiIO& io = ImGui::GetIO();
	mFontDefault = io.Fonts->AddFontDefault();
	mFontVox20 = io.Fonts->AddFontFromFileTTF("Fonts/Video.ttf", 20.0f);
}



void Gui::Draw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	DrawTopbar();
	DrawSidebar();
	DrawBottombar();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::DrawTopbar()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(mGame->GetTopbarWidth(), mGame->GetTopbarHeight()));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, mColorScheme.color1);
	ImGui::PushStyleColor(ImGuiCol_Text, mColorScheme.color2);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	{
		ImGui::Begin("TOPBAR", &mTopbarOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImGui::SetCursorPos(ImVec2(10, 4));
		ImGui::PushFont(mFontVox20);
		ImGui::Text("KAWASEMI");
		ImGui::PopFont();
		ImGui::End();
	}

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();
}

void Gui::DrawSidebar()
{
	ImGui::SetNextWindowPos(ImVec2(mGame->GetWindowWidth() - mGame->GetSidebarWidth(), mGame->GetTopbarHeight()));
	ImGui::SetNextWindowSize(ImVec2(mGame->GetSidebarWidth(), mGame->GetSidebarHeight()));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, mColorScheme.color1);
	ImGui::PushStyleColor(ImGuiCol_Text, mColorScheme.color2);

	{
		ImGui::Begin("SIDEBAR", &mSidebarOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		ImGui::PushFont(mFontVox20);
		ImGui::SeparatorText("Camera");

        ImGui::Text("Camera Position");
        std::string cameraPosition;
        glm::vec3 tmppos = mGame->GetCamera()->GetPosition();
        cameraPosition = std::to_string(tmppos.x) + std::string(", ") + std::to_string(tmppos.y) + std::string(", ") + std::to_string(tmppos.z);
        ImGui::Text(cameraPosition.c_str());

        ImGui::Text("Camera Direction");
        std::string cameraDirection;
        glm::vec3 tmpdir = mGame->GetCamera()->GetDirection();
        cameraDirection = std::to_string(tmpdir.x) + std::string(", ") + std::to_string(tmpdir.y) + std::string(", ") + std::to_string(tmpdir.z);
        ImGui::Text(cameraDirection.c_str());


        //
        ImGui::SeparatorText("Actors");
        ImGui::BeginListBox("Actors", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()));
        std::vector<Actor*> tmpActors = mGame->GetActors();
        for (int i = 0; i < tmpActors.size(); i++)
        {
            const bool is_selected = (i == mActorIndex);
            if (ImGui::Selectable(tmpActors[i]->GetName(), is_selected))
            {
                mActorIndex = i;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();


        //
        Actor* selectedActor = tmpActors[mActorIndex];
        ImGui::SeparatorText("Properties");
        ImGui::Text(selectedActor->GetName());

        if (selectedActor == mGame->GetCamera())
        {
            CameraActor* tmpCamera = mGame->GetCamera();

            float tmpMouseSpeed = tmpCamera->GetMouseSpeed();
            ImGui::DragFloat("Sensitivity", &tmpMouseSpeed, 0.001, 0.0f, 1.0f);
            tmpCamera->SetMouseSpeed(tmpMouseSpeed);

            float tmpMoveSpeed = tmpCamera->GetMoveSpeed();
            ImGui::DragFloat("MoveSpeed", &tmpMoveSpeed, 0.05f, 0.0f);
            tmpCamera->SetMoveSpeed(tmpMoveSpeed);

            float tmpFov = glm::degrees(tmpCamera->GetFov());
            ImGui::DragFloat("FOV", &tmpFov, 0.05f, 30.0f, 150.0f);
            tmpCamera->SetFov(glm::radians(tmpFov));

            float tmpNear = tmpCamera->GetNear();
            ImGui::DragFloat("Near", &tmpNear, 0.05f, 0.001f, 10.0f);
            tmpCamera->SetNear(tmpNear);

            float tmpFar = tmpCamera->GetFar();
            ImGui::DragFloat("Far", &tmpFar, 0.05f, 1.0f, 200.0f);
            tmpCamera->SetFar(tmpFar);
        }
        else
        {
            //ActorPosition
            glm::vec3 tmpPos = selectedActor->GetPosition();
            ImGui::DragFloat3("Position", &tmpPos[0], 0.05f);
            selectedActor->SetPosition(tmpPos);

            //ActorRotation
            glm::vec3 tmpRot = selectedActor->GetRotation();
            ImGui::DragFloat3("Rotation", &tmpRot[0], 0.05f);
            selectedActor->SetRotation(tmpRot);

            //ActorScale
            glm::vec3 tmpSca = selectedActor->GetScale();
            ImGui::DragFloat3("Scale", &tmpSca[0], 0.05, 0.0f, 10.0f);
            selectedActor->SetScale(tmpSca);
        }


		ImGui::PopFont();

		ImGui::End();
	}

	ImGui::PopStyleColor(2);
}

void Gui::DrawBottombar()
{
	ImGui::SetNextWindowPos(ImVec2(0, mGame->GetWindowHeight() - mGame->GetBottombarHeight()));
	ImGui::SetNextWindowSize(ImVec2(mGame->GetBottombarWidth(), mGame->GetBottombarHeight()));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, mColorScheme.color1);
	ImGui::PushStyleColor(ImGuiCol_Text, mColorScheme.color2);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	{
		ImGui::Begin("BOTTOMBAR", &mBottombarOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImGui::SetCursorPos(ImVec2(10, 4));
		ImGui::PushFont(mFontVox20);
		ImGui::Text("Window Size : (%d , %d)", mGame->GetWindowWidth(), mGame->GetWindowHeight());
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(mGame->GetWindowWidth() - 75, 4));
		ImGui::Text("%.1f FPS", double(ImGui::GetIO().Framerate));
		ImGui::PopFont();
		ImGui::End();
	}

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();
}

ImVec4 Gui::RGBAColor(int r, int g, int b, int a)
{
	return ImVec4(float(r) / 255, float(g) / 255, float(b) / 255, float(a) / 255);
}
