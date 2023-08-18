#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using json = nlohmann::json;


class Scene
{
public:

    struct PointlightCompStruct
    {
        float power;
        float near;
        float far;
        glm::vec3 position;
        glm::vec3 color;
    };

    struct TextureStruct
    {
        std::string path;
    };

    struct MaterialStruct
    {
        std::string name;
        TextureStruct texture;
        glm::vec3 color;
        bool useTexture;
    };

    struct MeshStruct
    {
        std::string path;
        bool smooth;
    };

    struct MeshCompStruct
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        MeshStruct mesh;
        MaterialStruct material;
    };

    struct ActorStruct
    {
        std::string name;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        std::vector<MeshCompStruct> meshComponents;
        std::vector<PointlightCompStruct> pointlightComponents;
    };

    struct CameraStruct
    {
        glm::vec3 position;
        glm::vec3 rotation;
        float fov;
        float near;
        float far;
        float moveSpeed;
        float mouseSpeed;
    };

    struct SceneStruct
    {
        CameraStruct camera;
        std::vector<ActorStruct> actors;
    };

    Scene();
    void LoadScene(const char* filepath);
    void SaveScene(const char* filepath, SceneStruct sceneStruct);
    SceneStruct GetSceneStructure() { return mSceneStruct; }

private:
    json mSceneJson;
    const char* mFilepath;
    SceneStruct mSceneStruct;

    Scene::SceneStruct Json2Struct_Scene(json sceneJson);
    json Struct2Json_Scene(SceneStruct sceneStruct);
    Scene::ActorStruct Json2Struct_Actor(json data);
    json Struct2Json_Actor(ActorStruct actor);

    json ReadJson(const char* filepath);
    void WriteJson(const char* filepath, json data);

    glm::vec3 vec2glm(std::vector<float> v);
    std::vector<float> glm2vec(glm::vec3 v);
};
