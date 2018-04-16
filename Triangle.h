#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Vertex
{
public:
	Vertex() {}
	Vertex(Vector3 v, ULONG c) : position(v), color(c) {}
	Vector3 position;
	ULONG color;
};

struct Triangle
{
public:
	Triangle() {}
	Vertex vertices[3];

	bool IsInTriangle(int x, int y, const Matrix3& mt);
};

