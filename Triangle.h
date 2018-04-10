#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Vertex
{
public :
	Vertex() { };
	Vector3 position;
	ULONG color;
};

struct Triangle
{
public:
	Triangle() {};

	Vector3 vertices[3];
	bool isInTriangle() { return false; }

	// Renderer에서 점 찍는 것을 삼각형으로 관리하여 그리게 작성
};

