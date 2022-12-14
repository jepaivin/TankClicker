#include "framework.h"
#include "View.h"


void View::Initialize()
{
	Brush_Background = CreateSolidBrush(RGB(123, 175, 128));
	Brush_Player = CreateSolidBrush(RGB(128, 255, 255));
	Brush_Enemy = CreateSolidBrush(RGB(128, 0, 0));
	Brush_EnemyOverlapping = CreateSolidBrush(RGB(64, 0, 0));
	Brush_EnemyInRange = CreateSolidBrush(RGB(192, 0, 0));
	Brush_EnemyBullet = CreateSolidBrush(RGB(255, 215, 0));
	Brush_TankBullet = CreateSolidBrush(RGB(0, 0, 0));
	Brush_Black = CreateSolidBrush(RGB(0, 0, 0));
	Brush_White = CreateSolidBrush(RGB(255, 255, 255));	
	Pen_Tank_Gun = CreatePen(0, 4, RGB(0,0,0));
	Pen_Click_Indicator = CreatePen(0, 1, RGB(255, 255, 255));	
}

void View::OnWindowChanged(HWND Window)
{
	// Re-create backbuffer

	if (BufferDC != nullptr)
	{
		ReleaseDC(WindowFromDC(BufferDC), BufferDC);
	}
	if (BufferBitmap != nullptr)
	{
		DeleteObject(BufferBitmap);
	}
	if (BackgroundBytes != nullptr)
	{
		delete[] BackgroundBytes;
	}

	HDC WindowDC = GetDC(Window);
	BufferDC = CreateCompatibleDC(WindowDC);

	RECT Rect;
	GetWindowRect(Window, &Rect);
	BufferWidth = Rect.right - Rect.left;
	BufferHeight = Rect.bottom - Rect.top;

	BackgroundBytes = new unsigned int[BufferWidth * BufferHeight];
	BufferBitmap = CreateCompatibleBitmap(WindowDC, BufferWidth, BufferHeight);
	SelectObject(BufferDC, BufferBitmap);
}

void View::FillBackgroundBuffer(unsigned int Color)
{
	for (int y = 0; y < BufferHeight; y++)
	{
		for (int x = 0; x < BufferWidth; x++)
		{
			BackgroundBytes[y * BufferWidth + x] = Color;
		}
	}

}
void View::DrawBackground(float DangerLevel)
{	
	unsigned int Color = 0x7baf80 + ((int)(DangerLevel * 48) << 8);
	FillBackgroundBuffer(Color);	

	BITMAPINFO info;
	ZeroMemory(&info, sizeof(BITMAPINFO));
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biWidth = BufferWidth;
	info.bmiHeader.biHeight = BufferHeight;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biSizeImage = BufferWidth * BufferHeight;
	info.bmiHeader.biCompression = BI_RGB;

	StretchDIBits(BufferDC, 0, 0, BufferWidth, BufferHeight, 0, 0, BufferWidth, BufferHeight, BackgroundBytes, &info, DIB_RGB_COLORS, SRCCOPY);
}

void View::Draw(const Game& GameInstance)
{
	// Draw the game view 

	HDC DC = BufferDC;

	// Draw background

	DrawBackground(GameInstance.GetDangerLevel());
	
	// Draw enemies

	for (int i = 0; i < GameInstance.NumEnemies; i++)
	{
		if (GameInstance.Enemies[i].Pos.X > BufferWidth || GameInstance.Enemies[i].Pos.Y > BufferHeight)
		{
			// Tank is off screen
			continue;
		}

		if (GameInstance.Enemies[i].InRange)
		{
			DrawTank(DC, GameInstance.Enemies[i].Pos, GameInstance.Enemies[i].Angle, Brush_EnemyInRange);
		}
		else
		{
			DrawTank(DC, GameInstance.Enemies[i].Pos, GameInstance.Enemies[i].Angle, Brush_Enemy);
		}
	}
	
	// Player
	DrawTank(DC, GameInstance.Player.Pos, GameInstance.Player.Angle, Brush_Player);
	
	// Click indicator
	DrawCircle(DC, GameInstance.MoveTarget, 8, Brush_White);
}


void View::SwapBuffers(HDC DC)
{
	RECT Rect;
	GetWindowRect(WindowFromDC(DC), &Rect);

	BitBlt(DC, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top,  BufferDC, 0, 0, SRCCOPY);
}

void View::DrawCircle(HDC DC, Vector2 Pos, int Radius, HBRUSH Brush)
{
	int X = (int)Pos.X;
	int Y = (int)Pos.Y;
	SetBkMode(DC, TRANSPARENT);
	SelectObject(DC, Pen_Click_Indicator);
	Ellipse(DC, X - Radius, Y - Radius, X + Radius, Y + Radius);
}

void View::DrawTank(HDC DC, Vector2 Pos, float Angle, HBRUSH Brush)
{
	RECT Rect;
	
	int X = (int)Pos.X;
	int Y = (int)Pos.Y;
	
	int Size = 12;

	SetRect(&Rect, X - Size, Y - Size, X + Size, Y + Size);
	
	FillRect(DC, &Rect, Brush);
	
	Vector2 LineEnd;
	LineEnd.X = Pos.X + cos(Angle) * 20;
	LineEnd.Y = Pos.Y + sin(Angle) * 20;
	
	SelectObject(DC, Pen_Tank_Gun);

	MoveToEx(DC, (int)Pos.X, (int)Pos.Y, nullptr);
	LineTo(DC, (int)LineEnd.X, (int)LineEnd.Y);
}