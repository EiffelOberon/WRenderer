#include "Texture.h"

std::map<std::string, std::shared_ptr<Texture>> Texture::textureMap;

Texture::Texture()
{

}

Texture::Texture(std::string& pathStr)
{
	this->path = GetBasePath(pathStr);
	this->fileName = GetFileName(pathStr);
}

Texture::~Texture()
{

}

void Texture::SetTextureID(GLuint id)
{
	this->textureID = id;
}

std::string Texture::GetBasePath(const std::string& path)
{
	size_t pos = path.find_last_of("\\/");
	return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
}

std::string Texture::GetFileName(const std::string& path)
{
	size_t pos = path.find_last_of("\\/");
	return (std::string::npos == pos) ? "" : path.substr(pos+1);
}