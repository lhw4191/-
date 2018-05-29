#include "stdafx.h"
#include "Vector.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Triangle.h"
#include "Texture.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Transform2D.h"

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
Sprite steveSprite;
Sprite creeperSprite;
Sprite** sprites;
int spritesSize;

void InitFrame(void)
{
	int vertexSize, indexSize;
	// 텍스쳐 초기화
	steveTexture = new Texture();
	steveTexture->LoadBMP("test.bmp");
	creeperTexture = new Texture();
	creeperTexture->LoadBMP("creeper.bmp");

	// 메테리얼 초기화
	steveSprite.SetDrawLayer(0);
	steveSprite.SetTexture(steveTexture);
	creeperSprite.SetDrawLayer(1);
	creeperSprite.SetTexture(creeperTexture);

	spritesSize = 2;
	sprites = new Sprite*[spritesSize];
	sprites[0] = &steveSprite;
	sprites[1] = &creeperSprite;

	std::sort(sprites, sprites + spritesSize, Sprite::comp);

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

	static Vector2 cameraOffset;
	static float cameraAngle;

	if (GetAsyncKeyState(VK_LSHIFT))
	{
		if (GetAsyncKeyState(VK_LEFT)) cameraOffset.X -= 1.0f;
		if (GetAsyncKeyState(VK_RIGHT)) cameraOffset.X += 1.0f;
		if (GetAsyncKeyState(VK_UP)) cameraOffset.Y += 1.0f;
		if (GetAsyncKeyState(VK_DOWN)) cameraOffset.Y -= 1.0f;
		if (GetAsyncKeyState(VK_PRIOR)) cameraAngle += 1.0f;
		if (GetAsyncKeyState(VK_NEXT)) cameraAngle -= 1.0f;
	}
	else
	{
		if (GetAsyncKeyState(VK_LEFT)) steveSprite.position.X -= 1.0f;
		if (GetAsyncKeyState(VK_RIGHT)) steveSprite.position.X += 1.0f;
		if (GetAsyncKeyState(VK_UP)) steveSprite.angle += 1.0f;
		if (GetAsyncKeyState(VK_DOWN)) steveSprite.angle -= 1.0f;
		if (GetAsyncKeyState(VK_PRIOR)) steveSprite.scale *= 1.01f;
		if (GetAsyncKeyState(VK_NEXT)) steveSprite.scale *= 0.99f;
	}
	
	// Layer Swap Test
	if (GetAsyncKeyState(VK_NUMPAD1))
	{
		steveSprite.SetDrawLayer(0);
		std::sort(sprites, sprites + spritesSize, Sprite::comp);
	}
	if (GetAsyncKeyState(VK_NUMPAD2))
	{
		steveSprite.SetDrawLayer(2);
		std::sort(sprites, sprites + spritesSize, Sprite::comp);
	}

	Transform2D QuadTransform;
	Matrix3 TRSMat;

	Transform2D CameraTransform(cameraOffset, cameraAngle, 1.0f);
	Matrix3 ViewMat = CameraTransform.GetViewMatrix();

	// Axis Draw
	Vector3 XStart(0.0f, 0.0f, 1.0f);
	Vector3 XEnd(0.0f, 0.0f, 1.0f);
	Vector3 YStart(0.0f, 0.0f, 1.0f);
	Vector3 YEnd(0.0f, 0.0f, 1.0f);

	XStart = XStart * ViewMat;
	XEnd = XEnd * ViewMat;
	YStart = YStart * ViewMat;
	YEnd = YEnd * ViewMat;

	XStart.X = (float)g_nClientWidth * -0.5f;
	XEnd.X = (float)g_nClientWidth * 0.5f;
	YStart.Y = (float)g_nClientHeight * -0.5f;
	YEnd.Y = (float)g_nClientHeight * 0.5f;

	SetColor(255, 0, 0);
	DrawLine(XStart, XEnd);
	SetColor(0, 255, 0);
	DrawLine(YStart, YEnd);
	
	// Sprite Draw
	ViewMat = CameraTransform.GetViewMatrix();

	for (int i = 0; i < spritesSize; i++)
	{
		QuadTransform.SetTrasform(sprites[i]->position, sprites[i]->angle, sprites[i]->scale);
		TRSMat = QuadTransform.GetTRSMatrix();

		sprites[i]->SetMatrix(TRSMat, ViewMat);
		sprites[i]->Render(&mesh);
	}

	// Buffer Swap 
	BufferSwap();
}
