#pragma once

#include <string>
#include <vector>

#include "SceneMesh.h"

class SceneNode
{
public:
	SceneNode(std::string name);
	~SceneNode();

	void AddChild(std::shared_ptr<SceneNode> child);
	void AddMesh(std::shared_ptr<SceneMesh> mesh);
	
private:
	std::string name;
	std::vector<std::shared_ptr<SceneNode>> children;
	std::vector<std::shared_ptr<SceneMesh>> meshes;
};