
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

#include "Vector.h"
#include "Matrix.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(int x, int y)
{
	if (!IsInRange(x, y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}

void DrawCircle(Vector2 cen_pos, float radius, const Matrix2 mat)
{
	int nradius = (int)radius;

	Vector2 checkPixel_pos;

	for (int x = cen_pos.X - nradius; x < cen_pos.X + nradius; x++)
	{
		for (int y = cen_pos.Y - nradius; y < cen_pos.Y + nradius; y++)
		{
			checkPixel_pos = Vector2(x, y);

			if (Vector2::Dist(cen_pos, checkPixel_pos) < radius)
			{
				checkPixel_pos = checkPixel_pos * mat;
				PutPixel(checkPixel_pos.X, checkPixel_pos.Y);
			}
		}
	}
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	Vector2 circle_Pos(0, 0);
	Matrix2 circle_ScaleMatrix2D;
	Matrix2 circle_RotateMatrix2D;

	Matrix2 circle_TransformationMatrix2D;

	static float angle = 0.0f;
	angle += 0.1f;

	circle_ScaleMatrix2D.SetIdentity();
	circle_ScaleMatrix2D.SetScale(3.0f, 1.0f);

	circle_RotateMatrix2D.SetIdentity();
	circle_RotateMatrix2D.Rotate(angle);

	circle_TransformationMatrix2D = circle_ScaleMatrix2D * circle_RotateMatrix2D;

	// Draw
	SetColor(255, 0, 0);
	DrawCircle(circle_Pos, 50.0f, circle_TransformationMatrix2D);

	SetColor(0, 0, 255);

	// Draw Line юс╫ц
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if (i == j)
			{
				Vector2 pos((float)i, (float)j);
				pos = pos * circle_RotateMatrix2D;
	
				PutPixel(pos.X, pos.Y);
			}
		}
	}

	// Buffer Swap 
	BufferSwap();
}
