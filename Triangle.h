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

	// Renderer���� �� ��� ���� �ﰢ������ �����Ͽ� �׸��� �ۼ�
};

