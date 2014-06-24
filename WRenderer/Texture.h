#pragma once

#include <map>

#include "GL\glew.h"

class Texture
{
public:
	Texture();
	Texture(std::string& pathStr);
	~Texture();

	void SetTextureID(GLuint id);

	static std::map<std::string, std::shared_ptr<Texture>> textureMap;
	static int GetNumberOfTextures()
	{
		return textureMap.size();
	}
private:
	std::string GetBasePath(const std::string& path);
	std::string GetFileName(const std::string& path);

	std::string path;
	std::string fileName;

	GLuint textureID;
};