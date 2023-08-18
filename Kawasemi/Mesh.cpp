#include "Mesh.h"

Mesh::Mesh(std::string path, bool smooth)
    : mPath(path)
    , mSmooth(smooth)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::fstream ifs(path, std::ios::in);
	if (!ifs)
	{
		std::cout << "unable to open obj file" << std::endl;
	}

	std::string line;
	std::string s;
	float tmp;

	while (std::getline(ifs, line))
	{
		std::stringstream ss(line);
		std::getline(ss, s, ' ');
		if (s == "v")
		{
			glm::vec3 vertex;
			std::getline(ss, s, ' ');
			vertex.x = std::stof(s);
			std::getline(ss, s, ' ');
			vertex.y = std::stof(s);
			std::getline(ss, s, ' ');
			vertex.z = std::stof(s);
			temp_vertices.push_back(vertex);
		}
		else if (s == "vt")
		{
			glm::vec2 uv;
			std::getline(ss, s, ' ');
			uv.x = std::stof(s);
			std::getline(ss, s, ' ');
			uv.y = std::stof(s);
			temp_uvs.push_back(uv);
		}
		else if (s == "vn")
		{
			glm::vec3 normal;
			std::getline(ss, s, ' ');
			normal.x = std::stof(s);
			std::getline(ss, s, ' ');
			normal.y = std::stof(s);
			std::getline(ss, s, ' ');
			normal.z = std::stof(s);
			temp_normals.push_back(normal);
		}
		else if (s == "f")
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			for (int i = 0; i < 3; i++)
			{
				std::getline(ss, s, ' ');
				std::stringstream sss(s);
				std::getline(sss, s, '/');
				vertexIndex[i] = std::stoi(s) - 1;
				std::getline(sss, s, '/');
				uvIndex[i] = std::stoi(s) - 1;
				std::getline(sss, s, '/');
				normalIndex[i] = std::stoi(s) - 1;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	if (smooth)
	{
		std::vector<glm::vec3> new_normals;
		for (int i = 0; i < temp_vertices.size(); i++) {
			int s = 0;
			glm::vec3 normal_sum = glm::vec3(0.0f, 0.0f, 0.0f);
			for (int j = 0; j < vertexIndices.size(); j++) {
				if (vertexIndices[j] == i) {
					s++;
					normal_sum += temp_normals[normalIndices[j]];
				}
			}
			normal_sum = glm::normalize(normal_sum / float(s));
			new_normals.push_back(normal_sum);
		}
		for (int i = 0; i < vertexIndices.size(); i++) {
			int vertexIndex = vertexIndices[i];
			int uvIndex = uvIndices[i];
			glm::vec3 vertex = temp_vertices[vertexIndex];
			glm::vec2 uv = temp_uvs[uvIndex];
			glm::vec3 normal = new_normals[vertexIndex];
			mVertices.push_back(vertex);
			mUvs.push_back(uv);
			mNormals.push_back(normal);
		}
	}
	else
	{
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 vertex = temp_vertices[vertexIndex];
			glm::vec2 uv = temp_uvs[uvIndex];
			glm::vec3 normal = temp_normals[normalIndex];
			mVertices.push_back(vertex);
			mUvs.push_back(uv);
			mNormals.push_back(normal);
		}
	}

	mNumOfVertices = mVertices.size();

	glGenBuffers(1, &mVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mNormalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalbuffer);
	glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mUvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mUvbuffer);
	glBufferData(GL_ARRAY_BUFFER, mUvs.size() * sizeof(glm::vec2), &mUvs[0], GL_STATIC_DRAW);

}