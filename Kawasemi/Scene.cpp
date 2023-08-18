#include "Scene.h"
#include "Actor.h"
#include "Component.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "LightComponent.h"

Scene::Scene()
{
}

void Scene::LoadScene(const char* filepath)
{
    std::cout << "Loading Scene from Json" << std::endl;

    mSceneJson = ReadJson(filepath);
    mSceneStruct = Json2Struct_Scene(mSceneJson);

    std::cout << "Scene Converted into Struct" << std::endl;
}

void Scene::SaveScene(const char* filepath, SceneStruct sceneStruct)
{
    std::cout << "Saving Scene" << std::endl;

    //mSceneStruct = sceneStruct;
    json data;
    data = Struct2Json_Scene(sceneStruct);
    WriteJson(filepath, data);

    std::cout << "Scene saved" << std::endl;
}

json Scene::Struct2Json_Scene(SceneStruct sceneStruct)
{
    json data;

    //Camera
    data["Camera"]["position"] = glm2vec(sceneStruct.camera.position);
    data["Camera"]["rotation"] = glm2vec(sceneStruct.camera.rotation);
    data["Camera"]["fov"] = sceneStruct.camera.fov;
    data["Camera"]["near"] = sceneStruct.camera.near;
    data["Camera"]["far"] = sceneStruct.camera.far;
    data["Camera"]["moveSpeed"] = sceneStruct.camera.moveSpeed;
    data["Camera"]["mouseSpeed"] = sceneStruct.camera.mouseSpeed;

    for (int i = 0; i < sceneStruct.actors.size(); i++) {
        ActorStruct actorStruct = sceneStruct.actors[i];
        json actorJson = Struct2Json_Actor(actorStruct);
        data["Actors"][std::to_string(i)] = actorJson;
    }

    return data;
}

Scene::SceneStruct Scene::Json2Struct_Scene(json sceneJson)
{
    SceneStruct sceneStruct;

    CameraStruct cameraStruct;
    cameraStruct.position = vec2glm(sceneJson["Camera"]["position"]);
    cameraStruct.rotation = vec2glm(sceneJson["Camera"]["rotation"]);
    cameraStruct.fov = sceneJson["Camera"]["fov"];
    cameraStruct.near = sceneJson["Camera"]["near"];
    cameraStruct.far = sceneJson["Camera"]["far"];
    cameraStruct.moveSpeed = sceneJson["Camera"]["moveSpeed"];
    cameraStruct.mouseSpeed = sceneJson["Camera"]["mouseSpeed"];

    sceneStruct.camera = cameraStruct;

    for (auto& el : sceneJson["Actors"].items()) {
        ActorStruct actorStruct;
        actorStruct = Json2Struct_Actor(el.value());
        sceneStruct.actors.push_back(actorStruct);
    }

    return sceneStruct;
}

Scene::ActorStruct Scene::Json2Struct_Actor(json data)
{
    ActorStruct actor;
    actor.name = data["name"];
    actor.position = vec2glm(data["position"]);
    actor.rotation = vec2glm(data["rotation"]);
    actor.scale = vec2glm(data["scale"]);
    for (auto& el : data["Components"].items())
    {
        if (el.key() == "MeshComponents") {
            for (auto& me : el.value().items())
            {
                MeshCompStruct meshcomp;
                MeshStruct mesh;
                MaterialStruct material;
                TextureStruct texture;
                
                mesh.path = me.value()["Mesh"]["path"];
                mesh.smooth = me.value()["Mesh"]["smooth"];

                texture.path = me.value()["Material"]["Texture"]["path"];
                material.texture = texture;
                material.color = vec2glm(me.value()["Material"]["color"]);
                material.useTexture = me.value()["Material"]["useTexture"];
                material.name = me.value()["Material"]["name"];

                meshcomp.position = vec2glm(me.value()["position"]);
                meshcomp.rotation = vec2glm(me.value()["rotation"]);
                meshcomp.scale = vec2glm(me.value()["scale"]);
                meshcomp.mesh = mesh;
                meshcomp.material = material;

                actor.meshComponents.push_back(meshcomp);
            }
        }
        else if (el.key() == "PointlightComponents")
        {
            for (auto& me : el.value().items())
            {
                PointlightCompStruct point;

                point.power = me.value()["power"];
                point.position = vec2glm(me.value()["position"]);
                point.color = vec2glm(me.value()["color"]);

                actor.pointlightComponents.push_back(point);
            }
        }
    }

    return actor;
}

json Scene::Struct2Json_Actor(ActorStruct actor)
{
    json data;
    data["name"] = actor.name;
    data["position"] = glm2vec(actor.position);
    data["rotation"] = glm2vec(actor.rotation);
    data["scale"] = glm2vec(actor.scale);

    data["Components"]["MeshComponents"] = {};
    data["Components"]["PointlightComponents"] = {};

    for (int i = 0; i < actor.meshComponents.size(); i++) {
        MeshCompStruct meshComp = actor.meshComponents[i];
        data["Components"]["MeshComponents"][std::to_string(i)]["position"] = glm2vec(meshComp.position);
        data["Components"]["MeshComponents"][std::to_string(i)]["rotation"] = glm2vec(meshComp.rotation);
        data["Components"]["MeshComponents"][std::to_string(i)]["scale"] = glm2vec(meshComp.scale);
        data["Components"]["MeshComponents"][std::to_string(i)]["Mesh"]["path"] = meshComp.mesh.path;
        data["Components"]["MeshComponents"][std::to_string(i)]["Mesh"]["smooth"] = meshComp.mesh.smooth;
        data["Components"]["MeshComponents"][std::to_string(i)]["Material"]["Texture"]["path"] = meshComp.material.texture.path;
        data["Components"]["MeshComponents"][std::to_string(i)]["Material"]["color"] = glm2vec(meshComp.material.color);
        data["Components"]["MeshComponents"][std::to_string(i)]["Material"]["useTexture"] = meshComp.material.useTexture;
        data["Components"]["MeshComponents"][std::to_string(i)]["Material"]["name"] = meshComp.material.name;
    }

    for (int i = 0; i < actor.pointlightComponents.size(); i++) {
        PointlightCompStruct pointlightCompStruct = actor.pointlightComponents[i];
        data["Components"]["PointlightComponents"][std::to_string(i)]["power"] = pointlightCompStruct.power;
        data["Components"]["PointlightComponents"][std::to_string(i)]["position"] = glm2vec(pointlightCompStruct.position);
        data["Components"]["PointlightComponents"][std::to_string(i)]["color"] = glm2vec(pointlightCompStruct.color);
        data["Components"]["PointlightComponents"][std::to_string(i)]["near"] = pointlightCompStruct.near;
        data["Components"]["PointlightComponents"][std::to_string(i)]["far"] = pointlightCompStruct.far;
    }

    return data;
}

json Scene::ReadJson(const char* filepath)
{
    std::cout << "Reading Json file." << std::endl;
    std::ifstream f(filepath);
    json data = json::parse(f);
    f.close();
    std::cout << "Reading Done" << std::endl;
    return data;
}

void Scene::WriteJson(const char* filepath, json data)
{
    std::cout << "Writing Json file" << std::endl;
    std::string tmpstr = data.dump();
    std::ofstream f(filepath);
    f << tmpstr;
    f.close();
    std::cout << "Writing Done" << std::endl;
}

glm::vec3 Scene::vec2glm(std::vector<float> v)
{
    glm::vec3 tmp;
    tmp.x = v[0];
    tmp.y = v[1];
    tmp.z = v[2];
    return tmp;
}

std::vector<float> Scene::glm2vec(glm::vec3 v)
{
    std::vector<float> tmp;
    tmp.push_back(v.x);
    tmp.push_back(v.y);
    tmp.push_back(v.z);
    return tmp;
}