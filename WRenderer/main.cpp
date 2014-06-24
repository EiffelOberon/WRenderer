#include <iostream>
#include <memory>

#include "GL\glew.h"
#include "WRenderer.h"
#include "WLoader.h"

#include <SFML\Window.hpp>
#include <SFML\System.hpp>

#include "Settings.h"

bool running = false;
std::shared_ptr<WRenderer> wRenderer = nullptr;
std::shared_ptr<WLoader> wLoader = nullptr;

bool initiated = false;

void InitLoaderThread()
{
	wLoader = std::make_shared<WLoader>(running);
	wLoader->Run();
}

void InitRenderThread() 
{
	wRenderer = std::make_shared<WRenderer>(running);
	wRenderer->Run();
}

void MainLoop()
{
	while(true)
	{
		sf::Event event;
		if(running)
		{
			if(!initiated)
			{
				if(wLoader != nullptr && wRenderer != nullptr &&
					wLoader->ContextCreated() && wRenderer->ContextCreated())
				{
					std::shared_ptr<WLoadItem> item = std::shared_ptr<WLoadItem>(new WLoadItem(std::string("E:\\Projects\\3DModels\\Apple\\apple.obj")));
					wLoader->AddWLoadItem(item);

					std::cout<<"Finished Loading"<<std::endl;
					initiated = true;
				}
			}

			sf::sleep(sf::milliseconds(16));
		}
		else 
		{
			break;
		}
	}
}

void Cleanup(sf::Thread& renderThread)
{
	renderThread.terminate();
	wRenderer = nullptr;
	wLoader = nullptr;
}


void main()
{
	running = true;
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	sf::Thread thread(&InitRenderThread);
	thread.launch();

	sf::Thread thread2(&InitLoaderThread);
	thread2.launch();

	MainLoop();

	Cleanup(thread);
}