#include "WLoader.h"

#include <iostream>
#include <fstream>

#include "Settings.h"
#include "GL\glew.h"


WLoader::WLoader(bool& running) : WContext(running)
{
	settings = new sf::ContextSettings();
	settings->depthBits = 0;
	settings->stencilBits = 0;
	settings->antialiasingLevel = 0;
	settings->majorVersion = 4;
	settings->minorVersion = 3;

	CreateContext();
}

WLoader::~WLoader()
{
	delete window;
	delete settings;
}

void WLoader::CreateContext()
{
	//create window
	window = new sf::Window (sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OpenGL", sf::Style::Default, *settings);

	window->setVerticalSyncEnabled(false);
	window->setActive(false);
	window->setVisible(false);

	glewExperimental = true;
	GLenum err = glewInit();
	while(err!=GL_NO_ERROR) {
		std::string error;

		switch(err) {
		case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		std::cerr << "GL_" << error.c_str()<<std::endl;
		err=glGetError();
	}

	created = true;

}

void WLoader::Loop()
{
	while(true)
	{
		sf::Event event;
		if(running)
		{
			if(loadingQueue.size() > 0)
			{
				std::shared_ptr<WLoadItem> loadItem = loadingQueue.front();

				window->setActive(true);
				Import3DFromFile(loadItem->GetPath().c_str());
				window->setActive(false);

				loadingQueue.pop();
			}

			sf::sleep(sf::milliseconds(100));
		}
		else 
		{
			break;
		}
	}
}

bool WLoader::Import3DFromFile(const std::string& pFile)
{
	std::ifstream fin(pFile);
	if(!fin.fail())
	{
		fin.close();
	}
	else
	{
		std::cout<< ("Couldn't open file: " ) << pFile << std::endl;
		return false;
	}


	const aiScene* scene = importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality);
	if(!scene)
	{
		std::cout<< (importer.GetErrorString()) << std::endl;
		return false;
	}

	AssimpParser p(scene, pFile);
	p.Parse();
}