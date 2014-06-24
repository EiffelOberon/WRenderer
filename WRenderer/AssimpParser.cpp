#include "AssimpParser.h"

#include "Texture.h"
#include "CheckError.h"

#include <IL/il.h>

#include <memory>

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
	LoadModel();
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

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		ilBindImage(0);
	}

	ilDeleteImages(numTextures, imageIds); /* Because we have already copied image data into texture data we can release memory used by image. */

	//Cleanup
	delete [] imageIds;
	imageIds = NULL;
}

void AssimpParser::LoadModel()
{
	aiNode* root = scene->mRootNode;
	ParseModel(root, nullptr);

}

void AssimpParser::ParseModel(aiNode* node, std::shared_ptr<SceneNode> sceneNode)
{
	std::shared_ptr<SceneNode> newNode = std::shared_ptr<SceneNode>(new SceneNode(std::string(node->mName.C_Str())));

	for(int i=0; i<node->mNumMeshes; i++)
	{
		int meshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[meshIndex];

		std::shared_ptr<SceneMesh> newMesh = std::shared_ptr<SceneMesh>(new SceneMesh());
		newNode->AddMesh(newMesh);

		ParseMesh(newMesh, mesh);
	}

	if(sceneNode != nullptr)
	{
		sceneNode->AddChild(newNode);
	}

	for(int i=0; i<node->mNumChildren; i++)
	{
		aiNode* child = node->mChildren[i];		
		ParseModel(child, newNode);
	}
}


void AssimpParser::ParseMesh(std::shared_ptr<SceneMesh> sceneMesh, aiMesh* mesh)
{
	int indexArraySize = mesh->mNumFaces * 3;
	sceneMesh->CreateIndexArray(indexArraySize);

	int count = 0;

	for(int i=0; i<mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);

		for(int j=0; j<face.mNumIndices; j++)
		{
			sceneMesh->SetIndexArray(count, face.mIndices[j]);
			count++;
		}
	}

	int vboSize = mesh->mNumVertices * 3;
	sceneMesh->CreateVertexArray(vboSize);

	count = 0;
	int uvCount = 0;
	for(int i=0; i<mesh->mNumVertices; i++)
	{
		aiVector3D vertex = mesh->mVertices[i];
		aiVector3D normal = mesh->mNormals[i];
		for(int j=0; i<3; i++)
		{
			if(j == 0) 
			{
				sceneMesh->SetVertexArray(count, vertex.x);
				sceneMesh->SetNormalArray(count, normal.x);
			}
			else if(j == 1) 
			{
				sceneMesh->SetVertexArray(count, vertex.y);
				sceneMesh->SetNormalArray(count, normal.y);
			}
			else if(j == 2) 
			{
				sceneMesh->SetVertexArray(count, vertex.z);
				sceneMesh->SetNormalArray(count, normal.z);
			}
			count++;
		}

		aiVector3D* texCoord = mesh->mTextureCoords[0];
		for(int j=0; j<2; j++)
		{
			if(j == 0) 
			{
				sceneMesh->SetUVArray(count, texCoord->x);
			}
			else if(j == 1) 
			{
				sceneMesh->SetUVArray(count, texCoord->y);
			}
			uvCount++;
		}
	}

	sceneMesh->GenVBO();
}

std::string AssimpParser::GetBasePath(const std::string& path)
{
	size_t pos = path.find_last_of("\\/");
	return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
}