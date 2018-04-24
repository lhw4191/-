#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh()
{
	vertex = 0;
	triangle = 0;
}

void Mesh::CreateTriangle(int vertexCount)
{
	triangleCount = vertexCount / 3;

	triangle = new Triangle[triangleCount];

	for (int i = 0; i < triangleCount; i++)
	{
		triangle[i] = Triangle(vertex[i * 3], vertex[i * 3 + 1], vertex[i * 3 + 2]);
	}
}