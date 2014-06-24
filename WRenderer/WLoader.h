#pragma once

#include "WContext.h"
#include "AssimpParser.h"
#include <queue>

#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

class WLoadItem
{
public:
	WLoadItem(std::string fPath)
	{
		this->fPath = fPath;
	}

	~WLoadItem()
	{

	}

	std::string GetPath()
	{
		return fPath;
	}

private:
	std::string fPath;
};

class WLoader : public WContext
{
public:
	WLoader(bool& running);
	~WLoader();

	virtual void Run()
	{
		Loop();
	}

	void AddWLoadItem(std::shared_ptr<WLoadItem> item)
	{
		loadingQueue.push(item);
	}

protected:
	virtual void CreateContext();

private:
	void Loop();
	bool Import3DFromFile(const std::string& pFile);

	std::queue<std::shared_ptr<WLoadItem>> loadingQueue;

	Assimp::Importer importer;
};

