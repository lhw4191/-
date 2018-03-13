
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

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

void DrawCircle(int cen_pox_x, int cen_pox_y, float radius)
{
	int nradius = (int)radius;

	for (int x = cen_pox_x - nradius; x < cen_pox_x + nradius; x++)
	{
		for (int y = cen_pox_y - nradius; y < cen_pox_y + nradius; y++)
		{
			if (sqrt(pow(cen_pox_x - x, 2) + pow(cen_pox_y - y, 2)) < radius)
				PutPixel(x, y);
		}
	}
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	SetColor(255, 0, 0);
	DrawCircle(100, 40, 60.0f);
	

	// Buffer Swap 
	BufferSwap();
}
