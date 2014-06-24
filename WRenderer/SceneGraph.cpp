#include "SceneGraph.h"

SceneGraph::SceneGraph(std::shared_ptr<SceneNode> root)
{
	this->root = root;
}

SceneGraph::~SceneGraph()
{

}