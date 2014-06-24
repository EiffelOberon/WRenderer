#include "AssimpParser.h"

#include "Texture.h"
#include "CheckError.h"

#include <IL/il.h>



AssimpParser::AssimpParser(const aiScene* scene, const std::string& path)
{
	this->scene = scene;
	this->path = path;
}

AssimpParser::~AssimpParser()
{

}

void AssimpParser::Parse()
{
	LoadTextures();
}

void AssimpParser::LoadTextures()
{
	ILboolean success;

	/* Before calling ilInit() version should be checked. */
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		/// wrong DevIL version ///
		std::string err_msg = "Wrong DevIL version. Old devil.dll in system32/SysWow64?";
		char* cErr_msg = (char *) err_msg.c_str();
		return;
	}

	ilInit(); /* Initialization of DevIL */
	if (scene->HasTextures()) return;

	for (unsigned int m=0; m<scene->mNumMaterials; m++)
	{
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;

		aiString path;	// filename

		while (texFound == AI_SUCCESS)
		{
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			//textureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
			if(texFound == AI_SUCCESS)
			{
				std::string pathStr = std::string(path.C_Str());
				Texture::textureMap.insert(std::pair<std::string, std::shared_ptr<Texture>>(pathStr, std::shared_ptr<Texture>(new Texture(pathStr))));
			}

			texIndex++;
		}
	}

	int numTextures = Texture::GetNumberOfTextures();

	ILuint* imageIds = NULL;
	imageIds = new ILuint[numTextures];
	ilGenImages(numTextures, imageIds); /* Generation of numTextures image names */

	textureIds = new GLuint[numTextures];
	glGenTextures(numTextures, textureIds); /* Texture name generation */

	std::map<std::string, std::shared_ptr<Texture>>::iterator itr = Texture::textureMap.begin();

	std::string basepath = GetBasePath(path);
	for (int i=0; i<numTextures; i++)
	{
		std::string filename = (*itr).first;  // get filename
		(*itr).second->SetTextureID(textureIds[i]);	  // save texture id for filename in map
		itr++;								  // next texture

		ilBindImage(imageIds[i]); /* Binding of DevIL image name */
		std::string fileloc = basepath + filename;	/* Loading of image */
		success = ilLoadImage(fileloc.c_str());

		if (success) /* If no error occured: */
		{
			if(ilGetInteger(IL_IMAGE_FORMAT) == IL_RGB)
			{
				success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
			}
			else if(ilGetInteger(IL_IMAGE_FORMAT) == IL_RGBA)
			{
				success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
			}

			if(!success)
			{
				return;
			}

			glBindTexture(GL_TEXTURE_2D, textureIds[i]); /* Binding of texture name */
			GLenum e = glGetError();
			//redefine standard texture values
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
			interpolation for magnification filter */
			e = glGetError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
			interpolation for minifying filter */
			e = glGetError();
			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
				ilGetData()); /* Texture specification */
			e = glGetError();
		}
	}
}


std::string AssimpParser::GetBasePath(const std::string& path)
{
	size_t pos = path.find_last_of("\\/");
	return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
}