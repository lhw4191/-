#include "stdafx.h"
#include "Vector.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Triangle.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"

#include <algorithm>
using namespace std;

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

Mesh mesh;

Texture* steveTexture;
Texture* creeperTexture;
Material steveMaterial;
Material creeperMaterial;
Material** materials;
int materialSize;

void InitFrame(void)
{
	int vertexSize, indexSize;
	// 텍스쳐 초기화
	steveTexture = new Texture();
	steveTexture->LoadBMP("test.bmp");
	creeperTexture = new Texture();
	creeperTexture->LoadBMP("creeper.bmp");

	// 메테리얼 초기화
	steveMaterial.drawLayer = 0;
	steveMaterial.SetTexture(steveTexture);
	creeperMaterial.drawLayer = 1;
	creeperMaterial.SetTexture(creeperTexture);

	materialSize = 2;
	materials = new Material*[materialSize];
	materials[0] = &steveMaterial;
	materials[1] = &creeperMaterial;

	std::sort(materials, materials+ materialSize, Material::comp);

	// 메쉬 초기화
	// 점 생성
	Vector3 p1, p2, p3, p4;
	
	p1.SetPoint(-50.0f, 50.0f);  // 왼위
	p2.SetPoint(50.0f, 50.0f);   // 오위
	p3.SetPoint(50.0f, -50.0f);  // 오아래
	p4.SetPoint(-50.0f, -50.0f); // 왼아래
	
	vertexSize = 4;
	indexSize = 6;

	// Vertex
	mesh.vertexSize = vertexSize;
	mesh.vertices = new Vertex[vertexSize];
	mesh.vertices[0].position = p1;// 왼위
	mesh.vertices[0].uv = Vector2(0.125f, 0.125f);
	mesh.vertices[0].color = RGB(255, 255, 255);

	mesh.vertices[1].position = p2;// 오위
	mesh.vertices[1].uv = Vector2(0.25f, 0.125f);
	mesh.vertices[1].color = RGB(255, 255, 255);

	mesh.vertices[2].position = p3;// 오아래
	mesh.vertices[2].uv = Vector2(0.25f, 0.25f);
	mesh.vertices[2].color = RGB(255, 255, 255);

	mesh.vertices[3].position = p4;// 왼아래
	mesh.vertices[3].uv = Vector2(0.125f, 0.25f);
	mesh.vertices[3].color = RGB(255, 255, 255);

	// Index
	mesh.indexSize = indexSize;
	mesh.indices = new unsigned int[indexSize];
	mesh.indices[0] = 0;
	mesh.indices[1] = 1;
	mesh.indices[2] = 2;
	mesh.indices[3] = 2;
	mesh.indices[4] = 3;
	mesh.indices[5] = 0;
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

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

	steveMaterial.SetMatrix(TRSMat);

	for (int i = 0; i < materialSize; i++)
	{
		materials[i]->Render(&mesh);
	}

	// Buffer Swap 
	BufferSwap();
}
