#include "WRenderer.h"

#include <iostream>

#include "Settings.h"
#include "GL\glew.h"

WRenderer::WRenderer(bool& running) : WContext(running)
{
	settings = new sf::ContextSettings();
	settings->depthBits = 24;
	settings->stencilBits = 8;
	settings->antialiasingLevel = 4;
	settings->majorVersion = 4;
	settings->minorVersion = 3;

	CreateContext();
}

WRenderer::~WRenderer()
{
	delete window;
	delete settings;
}

void WRenderer::CreateContext()
{
	//create window
	window = new sf::Window (sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OpenGL", sf::Style::Default, *settings);

	window->setVerticalSyncEnabled(true);
	window->setActive(true);
	window->setVisible(true);

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



void WRenderer::PollInput() 
{
	sf::Event event;

	while(window->pollEvent(event))
	{

		if (event.type == sf::Event::Closed)
		{
			// end the program
			running = false;
		}
		else if(event.type == sf::Event::Resized)
		{
			glViewport(0, 0, event.size.width, event.size.height);
		}

	}
}

void WRenderer::RenderLoop()
{
	while(true)
	{
		sf::Event event;
		if(running)
		{
			PollInput();
			Render();

			// end the current frame -- this is a rendering function (it requires the context to be active)
			window->display();

			sf::sleep(sf::milliseconds(16));
		}
		else 
		{
			break;
		}
	}
}

void WRenderer::Render()
{
	glClearColor(1, 0, 0, 0);

	//clear color and depth buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

}