
#include "stdafx.h"
#include "Vector.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Triangle.h"
#include "Texture.h"
#include "Mesh.h"

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(IntPoint pt)
{
	if (!IsInRange(pt.X, pt.Y)) return;

	ULONG* dest = (ULONG*)g_pBits;

	int halfWidth = RoundToInt(g_nClientWidth * 0.5f);
	int halfHeight = RoundToInt(g_nClientHeight * 0.5f);

	DWORD offset = (halfHeight * g_nClientWidth - g_nClientWidth * pt.Y) + (halfWidth + pt.X);
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

void Draw2DTriangle(Triangle t)
{
	for (int y = t.Min.Y; y <= t.Max.Y; y++)
	{
		for (int x = t.Min.X; x <= t.Max.X; x++)
		{
			Vector3 target((float)x + 0.5f, (float)y + 0.5f, 0.0f);
			float outS, outT;
			t.CalcBaryCentricCoord(target, &outS, &outT);
			if (t.IsInTrianble(outS, outT))
			{
				if (g_Texture->IsLoaded())
				{
					g_CurrentColor = g_Texture->GetTexturePixel(outS, outT, t);
				}
				else
				{
					g_CurrentColor = t.GetPixelColor(target, outS, outT);
				}

				PutPixel(IntPoint(x, y));
			}			
		}
	}
}
void Draw2DMesh(Mesh mesh)
{
	for (int i = 0; i < mesh.triangleCount; i++)
	{
		for (int y = mesh.triangle[i].Min.Y; y <= mesh.triangle[i].Max.Y; y++)
		{
			for (int x = mesh.triangle[i].Min.X; x <= mesh.triangle[i].Max.X; x++)
			{
				Vector3 target((float)x + 0.5f, (float)y + 0.5f, 0.0f);
				float outS, outT;
				mesh.triangle[i].CalcBaryCentricCoord(target, &outS, &outT);
				if (mesh.triangle[i].IsInTrianble(outS, outT))
				{
					if (g_Texture->IsLoaded())
					{
						g_CurrentColor = g_Texture->GetTexturePixel(outS, outT, mesh.triangle[i]);
					}
					else
					{
						g_CurrentColor = mesh.triangle[i].GetPixelColor(target, outS, outT);
					}

					PutPixel(IntPoint(x, y));
				}
			}
		}
	}
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	Vector3 Pt1, Pt2, Pt3, Pt4, Pt5, Pt6;
	Mesh mesh;

	static float offsetX = 0.0f;
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

	// 점 생성
	Pt1.SetPoint(-150, 150.0f); // 왼위
	Pt2.SetPoint(150.0f, 150.0f); // 오위
	Pt3.SetPoint(150.0f, -150.0f); // 오아래
	Pt4.SetPoint(-150.0f, -150.0f); // 왼아래

	// 정점 배열 생성
	int vertexCount = 6;
	mesh.vertex = new Vertex[vertexCount];

	// 정점 배열 설정
	mesh.vertex[0] = Vertex(Pt1 * TRSMat);
	mesh.vertex[0].color = RGB32(255, 0, 0);
	mesh.vertex[0].uv = Vector2(0.0f, 0.0f);

	mesh.vertex[1] = Vertex(Pt2 * TRSMat);
	mesh.vertex[1].color = RGB32(0, 255, 0);
	mesh.vertex[1].uv = Vector2(1.0f, 0.0f);

	mesh.vertex[2] = Vertex(Pt3 * TRSMat);
	mesh.vertex[2].color = RGB32(0, 0, 255);
	mesh.vertex[2].uv = Vector2(1.0f, 1.0f);

	mesh.vertex[3] = Vertex(Pt1 * TRSMat);
	mesh.vertex[3].color = RGB32(128, 128, 0);
	mesh.vertex[3].uv = Vector2(0.0f, 0.0f);

	mesh.vertex[4] = Vertex(Pt3 * TRSMat);
	mesh.vertex[4].color = RGB32(0, 128, 128);
	mesh.vertex[4].uv = Vector2(1.0f, 1.0f);

	mesh.vertex[5] = Vertex(Pt4 * TRSMat);
	mesh.vertex[5].color = RGB32(128, 0, 128);
	mesh.vertex[5].uv = Vector2(0.0f, 1.0f);

	// 정점 배열 데이터를 이용하여 삼각형 배열 생성
	mesh.CreateTriangle(vertexCount);

	// 그리기
	Draw2DMesh(mesh);

	// Buffer Swap 
	BufferSwap();
}
