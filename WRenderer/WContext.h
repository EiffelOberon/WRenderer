#pragma once

#include <SFML\Window.hpp>
#include <SFML\System.hpp>

class WContext
{
public:
	WContext()
	{
		created = false;
	}
	WContext(bool& running)
	{
		created = false;
		this->running = running;
	}
	~WContext(){}

	bool ContextCreated()
	{
		return created;
	}

	virtual void Run() = 0;

protected:
	virtual void CreateContext() = 0;

	bool running;
	sf::ContextSettings* settings;
	sf::Window* window;

	bool created;
};