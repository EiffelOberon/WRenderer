#pragma once

#include <string>
#include <vector>

#include "GL/glew.h"

class SceneMesh
{
public:
	SceneMesh();
	~SceneMesh();

	void GenVBO();

	void CreateIndexArray(int length)
	{
		indexArray = std::unique_ptr<unsigned int[]>(new unsigned int[length]);
	}
	void CreateVertexArray(int length)
	{
		int posArraySize = length * 3;
		vertexPosArray = std::unique_ptr<float[]>(new float[posArraySize]);
		vertexNormArray = std::unique_ptr<float[]>(new float[posArraySize]);

		int uvArraySize = length * 2;
		vertexUVArray = std::unique_ptr<float[]>(new float[uvArraySize]);

		numOfTriangles = length / 3;
	}

	void SetIndexArray(int index, unsigned int val)
	{
		indexArray[index] = val;
	}

	void SetVertexArray(int index, float val)
	{
		vertexPosArray[index] = val;
	}

	void SetNormalArray(int index, float val)
	{
		vertexNormArray[index] = val;
	}

	void SetUVArray(int index, float val)
	{
		vertexUVArray[index] = val;
	}
private:
	std::unique_ptr<unsigned int[]> indexArray;
	std::unique_ptr<float[]> vertexPosArray;
	std::unique_ptr<float[]> vertexNormArray;
	std::unique_ptr<float[]> vertexUVArray;

	std::unique_ptr<GLuint[]> vboIDs;
	GLuint vaoID;

	int numOfTriangles;
};