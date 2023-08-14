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
    , mShadowResolutionPoint(1024)
{
}

void Renderer::Initialize()
{
	mCamera = mGame->GetCamera();

    glGenVertexArrays(1, &mTmpVertexArray);
    glBindVertexArray(mTmpVertexArray);

    glGenFramebuffers(1, &mShadowFBOPoint);
    glGenTextures(1, &mShadowTexturePoint);
}

void Renderer::CompileShaders()
{
    std::vector<std::string> rep_from_point;
    std::vector<std::string> rep_to_point;
    rep_from_point.push_back("NUMVERTICES");
    rep_from_point.push_back("MAXVERTICES");
    rep_from_point.push_back("SHADOWMATRICES");
    rep_to_point.push_back(std::to_string(mPointlights.size()));
    rep_to_point.push_back(std::to_string(mPointlights.size()*18));
    rep_to_point.push_back(std::to_string(mPointlights.size()*6));
    mShadowShaderPoint = new Shader("Shaders/ShadowPoint.vertex", "Shaders/ShadowPoint.fragment", "Shaders/ShadowPoint.geometry", rep_from_point, rep_to_point);


    std::vector<std::string> rep_from;
    std::vector<std::string> rep_to;
    rep_from.push_back(std::string("POINTNUM"));
    rep_to.push_back(std::to_string(mPointlights.size()));
	mSceneShader = new Shader("Shaders/Simple.vertex", "Shaders/Simple.fragment", nullptr, rep_from, rep_to);
}

void Renderer::PrepareShadowFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mShadowFBOPoint);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, mShadowTexturePoint);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT, mShadowResolutionPoint, mShadowResolutionPoint, GLuint(6 * mPointlights.size()), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mShadowTexturePoint, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    const int statusPoint = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (statusPoint != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Point FBO Not Ready" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawShadow()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mShadowFBOPoint);
    glViewport(0, 0, mShadowResolutionPoint, mShadowResolutionPoint);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mShadowShaderPoint->Use();
    for (int i = 0; i < mPointlights.size(); i++)
    {
        for (int j = 0; j < 6; j++)
        {
            std::string tmpstr = "shadow_matrices[" + std::to_string(i * 6 + j) + "]";
            mShadowShaderPoint->ConfigureMat4(tmpstr.c_str(), &mPointlights[i]->GetShadowTransforms()[j][0][0]);
        }
        mShadowShaderPoint->ConfigureVec3(("light_pos[" + std::to_string(i) + "]").c_str(), &mPointlights[i]->GetAbsPosition()[0]);
        mShadowShaderPoint->ConfigureFloat(("far_plane[" + std::to_string(i) + "]").c_str(), mPointlights[i]->GetFar());
    }
    for (auto mesh : mMeshes)
    {
        mShadowShaderPoint->ConfigureMat4("model", &mesh->GetModelMatrix()[0][0]);
        mShadowShaderPoint->ConfigureAttrib("pos", mesh->GetMesh()->GetVertexbuffer(), 3);
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetMesh()->GetNumOfVertices());
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, mGame->GetBottombarHeight(), mGame->GetViewportWidth(), mGame->GetViewportHeight());
	glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	mSceneShader->Use();
	mSceneShader->ConfigureMat4("camera_vp", &mCamera->GetVPMatrix()[0][0]);

    mSceneShader->ConfigureCubeArray("depthmap", mShadowTexturePoint, 0);
    for (int i = 0; i < mPointlights.size(); i++)
    {
        PointlightComponent* light = mPointlights[i];
        mSceneShader->ConfigureFloat(("light_pow[" + std::to_string(i) + "]").c_str(), light->GetPower());
        mSceneShader->ConfigureVec3(("light_pos[" + std::to_string(i) + "]").c_str(), &light->GetAbsPosition()[0]);
        mSceneShader->ConfigureVec3(("light_col[" + std::to_string(i) + "]").c_str(), &light->GetColor()[0]);
        mSceneShader->ConfigureFloat(("far_plane[" + std::to_string(i) + "]").c_str(), light->GetFar());
    }

	for (auto mesh : mMeshes)
	{
		mSceneShader->ConfigureMat4("model", &mesh->GetModelMatrix()[0][0]);
        mSceneShader->ConfigureMat4("rotation", &mesh->GetRotationMatrix()[0][0]);
		mSceneShader->ConfigureAttrib("pos", mesh->GetMesh()->GetVertexbuffer(), 3);
        mSceneShader->ConfigureAttrib("uv", mesh->GetMesh()->GetUvbuffer(), 2);
        mSceneShader->ConfigureAttrib("norm", mesh->GetMesh()->GetNormalbuffer(), 3);
        mSceneShader->ConfigureTexture("mesh_texture", mesh->GetMaterial()->GetTextureID(), 2);
		mSceneShader->ConfigureVec3("mesh_col", &mesh->GetMaterial()->GetColor()[0]);
        mSceneShader->ConfigureInt("use_texture", mesh->GetMaterial()->UseTexture());
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetMesh()->GetNumOfVertices());
	}
}