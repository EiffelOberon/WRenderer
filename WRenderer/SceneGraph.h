#pragma once

#include "SceneNode.h"
#include <memory>

class SceneGraph
{
public:
	SceneGraph(std::shared_ptr<SceneNode> root);
	~SceneGraph();

private:
	std::shared_ptr<SceneNode> root;
};