
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

#include "Vector.h"

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

void DrawCircle(Vector2 cen_pos, float radius)
{
	int nradius = (int)radius;

	Vector2 checkPixel_pos;

	for (int x = cen_pos.X - nradius; x < cen_pos.X + nradius; x++)
	{
		for (int y = cen_pos.Y - nradius; y < cen_pos.Y + nradius; y++)
		{
			checkPixel_pos = Vector2(x, y);

			if (Vector2::Dist( cen_pos, checkPixel_pos ) < radius)
				PutPixel(x, y);
		}
	}
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	Vector2 circle_Pos(0, 0);

	// Draw
	SetColor(255, 0, 0);
	DrawCircle(circle_Pos, 60.0f);

	// Buffer Swap 
	BufferSwap();
}
