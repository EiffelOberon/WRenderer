#pragma once

#include "GL\glew.h"

#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

#include "SceneNode.h"
#include "SceneMesh.h"

class AssimpParser
{
public:
	AssimpParser(const aiScene* scene, const std::string& path);
	~AssimpParser();

	void Parse();

private:
	void LoadTextures();
	void LoadModel();
	void ParseModel(aiNode* node, std::shared_ptr<SceneNode> sceneNode);
	void ParseMesh(std::shared_ptr<SceneMesh> sceneMesh, aiMesh* mesh);

	std::string GetBasePath(const std::string& path);

	const aiScene* scene;
	GLuint* textureIds;
	std::string path;
};