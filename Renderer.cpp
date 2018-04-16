
#include "stdafx.h"
#include "Vector.h"
#include "Triangle.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(IntPoint pt)
{
	if (!IsInRange(pt.X, pt.Y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + pt.X + g_nClientWidth * -pt.Y;
	*(dest + offset) = g_CurrentColor;
}

void DrawLine(const Vector3& start, const Vector3& end)
{
	float length = (end - start).Dist();
	float inc = 1.0f / length;

	int maxLength = RoundToInt(length);
	for (int i = 0; i <= maxLength; i++)
	{
		float t = inc * i;
		if (t >= length) t = 1.0f;
		Vector3 Pt = start * (1.0f - t) + end * t;
		PutPixel(Pt.ToIntPoint());
	}

}

void Draw2DTriangle(Triangle& triangle, const Matrix3& mt)
{
	float xMin, yMin;
	float xMax, yMax;
	xMin = yMin = INFINITY;
	xMax = yMax = -INFINITY;

	Vector3 v1, v2, v3;
	v1 = triangle.vertices[0].position * mt;
	v2 = triangle.vertices[1].position * mt;
	v3 = triangle.vertices[2].position * mt;

	if (v1.X < xMin) xMin = v1.X;
	if (v2.X < xMin) xMin = v2.X;
	if (v3.X < xMin) xMin = v3.X;
	if (v1.X > xMax) xMax = v1.X;
	if (v2.X > xMax) xMax = v2.X;
	if (v3.X > xMax) xMax = v3.X;
	if (v1.Y < yMin) yMin = v1.Y;
	if (v2.Y < yMin) yMin = v2.Y;
	if (v3.Y < yMin) yMin = v3.Y;
	if (v1.Y > yMax) yMax = v1.Y;
	if (v2.Y > yMax) yMax = v2.Y;
	if (v3.Y > yMax) yMax = v3.Y;

	for (int y = RoundToInt(yMin); y < RoundToInt(yMax); y++)
	{
		for (int x = RoundToInt(xMin); x < RoundToInt(xMax); x++)
		{
			if(triangle.IsInTriangle(x, y, mt))
				PutPixel(IntPoint(x, y));
		}
	}
}


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(50, 50, 50);
	Clear();

	// Draw
	Triangle triangle;

	static float offsetX = 0.0f;
	//static float offsetY = 0.0f;
	static float angle = 0.0f;
	static float scale = 1.0f;

	if (GetAsyncKeyState(VK_LEFT)) offsetX -= 1.0f;
	if (GetAsyncKeyState(VK_RIGHT)) offsetX += 1.0f;
	if (GetAsyncKeyState(VK_UP)) angle += 1.0f;
	if (GetAsyncKeyState(VK_DOWN)) angle -= 1.0f;
	if (GetAsyncKeyState(VK_PRIOR)) scale *= 1.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale *= 0.99f;

	Matrix3 TMat, RMat, SMat;
	TMat.SetTranslation(offsetX, 0.0f);
	RMat.SetRotation(angle);
	SMat.SetScale(scale);
	Matrix3 TRSMat = TMat * RMat * SMat;

	// Triangle Vertex Setting
	triangle.vertices[0].position.SetPoint(0.0f, 0.0f);
	triangle.vertices[1].position.SetPoint(160.0f, 160.0f);
	triangle.vertices[2].position.SetPoint(-20.0f, 160.0f);

	SetColor(255, 0, 0);
	Draw2DTriangle(triangle, TRSMat);

	// Buffer Swap 
	BufferSwap();
}
