#pragma once

#include "WContext.h"

class WRenderer : public WContext
{
public:
	WRenderer(bool& running);
	~WRenderer();

	virtual void Run()
	{
		RenderLoop();
	}

protected:
	virtual void CreateContext();
	
private:
	void PollInput();
	void RenderLoop();

	void Render();
};