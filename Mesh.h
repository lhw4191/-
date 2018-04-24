#pragma once
#include "Triangle.h"
#include "Matrix.h"

struct Mesh
{
public:
	Mesh();

	Vertex* vertex = 0;
	Triangle* triangle = 0;

	int triangleCount;
	void CreateTriangle(int vertexCount);
};