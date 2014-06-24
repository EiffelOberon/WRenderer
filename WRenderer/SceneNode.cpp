#include "SceneNode.h"

SceneNode::SceneNode(std::string name)
{
	this->name = name;
}

SceneNode::~SceneNode()
{
}


void SceneNode::AddChild(std::shared_ptr<SceneNode> child)
{
	children.push_back(child);
}

void SceneNode::AddMesh(std::shared_ptr<SceneMesh> mesh)
{
	meshes.push_back(mesh);
}