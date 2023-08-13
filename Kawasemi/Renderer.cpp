#include "Renderer.h"
#include "Game.h"
#include "Shader.h"
#include "CameraActor.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "LightComponent.h"

Renderer::Renderer(Game* game)
	: mGame(game)
	, mWindow(mGame->GetWindow())
	, mClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
{
}

void Renderer::Initialize()
{
	mCamera = mGame->GetCamera();

    glGenVertexArrays(1, &mTmpVertexArray);
    glBindVertexArray(mTmpVertexArray);
}

void Renderer::CompileShaders()
{
    std::vector<std::string> rep_from;
    std::vector<std::string> rep_to;
    rep_from.push_back(std::string("POINTNUM"));
    rep_to.push_back(std::to_string(mPointlights.size()));
	mSceneShader = new Shader("Shaders/Simple.vs", "Shaders/Simple.fs", nullptr, rep_from, rep_to);
}

void Renderer::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, mGame->GetBottombarHeight(), mGame->GetViewportWidth(), mGame->GetViewportHeight());
	glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	mSceneShader->Use();
	mSceneShader->ConfigureMat4("camera_vp", &mCamera->GetVPMatrix()[0][0]);

    for (int i = 0; i < mPointlights.size(); i++)
    {
        PointlightComponent* light = mPointlights[i];
        mSceneShader->ConfigureFloat(("light_pow[" + std::to_string(i) + "]").c_str(), light->GetPower());
        mSceneShader->ConfigureVec3(("light_pos[" + std::to_string(i) + "]").c_str(), &light->GetAbsPosition()[0]);
        mSceneShader->ConfigureVec3(("light_col[" + std::to_string(i) + "]").c_str(), &light->GetColor()[0]);
    }

	for (auto mesh : mMeshes)
	{
		mSceneShader->ConfigureMat4("model", &mesh->GetModelMatrix()[0][0]);
        mSceneShader->ConfigureMat4("rotation", &mesh->GetRotationMatrix()[0][0]);
		mSceneShader->ConfigureAttrib("pos", mesh->GetMesh()->GetVertexbuffer(), 3);
        mSceneShader->ConfigureAttrib("uv", mesh->GetMesh()->GetUvbuffer(), 2);
        mSceneShader->ConfigureAttrib("norm", mesh->GetMesh()->GetNormalbuffer(), 3);
        mSceneShader->ConfigureTexture("mesh_texture", mesh->GetMaterial()->GetTextureID(), 0);
		mSceneShader->ConfigureVec3("mesh_col", &mesh->GetMaterial()->GetColor()[0]);
        mSceneShader->ConfigureInt("use_texture", mesh->GetMaterial()->UseTexture());
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetMesh()->GetNumOfVertices());
	}

    //std::cout << mCamera->GetDirection().x << ", " << mCamera->GetDirection().y << ", " << mCamera->GetDirection().z << std::endl;
}