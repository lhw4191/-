#include "stdafx.h"
#include "Triangle.h"

bool Triangle::IsInTriangle(int x, int y, const Matrix3& mt)
{
	Vector2 u = (vertices[1].position * mt - vertices[0].position * mt).ToVector2();
	Vector2 v = (vertices[2].position * mt - vertices[0].position * mt).ToVector2();
	float dotUU = u.Dot(u);
	float dotUV = u.Dot(v);
	float dotVV = v.Dot(v);
	float invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);

	Vector2 w = (Vector3((float)x, (float)y, 0.0f) - vertices[0].position * mt).ToVector2();
	float dotUW = u.Dot(w);
	float dotVW = v.Dot(w);
	float outS = (dotVV * dotUW - dotUV * dotVW) * invDenom;
	float outT = (dotUU * dotVW - dotUV * dotUW) * invDenom;

	if (outS < 0.0f) return false;
	if (outT < 0.0f) return false;
	if (outS + outT > 1.0f) return false;

	return true;
}
 