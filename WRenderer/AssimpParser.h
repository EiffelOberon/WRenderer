#pragma once

#include "GL\glew.h"

#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

class AssimpParser
{
public:
	AssimpParser(const aiScene* scene, const std::string& path);
	~AssimpParser();

	void Parse();

private:
	void LoadTextures();
	std::string GetBasePath(const std::string& path);

	const aiScene* scene;
	GLuint* textureIds;
	std::string path;
};