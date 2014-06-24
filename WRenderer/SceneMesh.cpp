#include "SceneMesh.h"

SceneMesh::SceneMesh()
{
	vboIDs = std::unique_ptr<GLuint[]>(new GLuint[3]);
}

SceneMesh::~SceneMesh()
{
	indexArray.reset();

	vertexPosArray.reset();
	vertexNormArray.reset();
	vertexUVArray.reset();
}

void SceneMesh::GenVBO()
{
	glGenBuffers(3, vboIDs.get());

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, numOfTriangles * 3 * sizeof(GL_FLOAT), vertexPosArray.get(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, numOfTriangles * 3 * sizeof(GL_FLOAT), vertexNormArray.get(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, numOfTriangles * 2 * sizeof(GL_FLOAT), vertexUVArray.get(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *) nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *) nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *) nullptr);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}