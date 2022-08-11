#pragma once
#include "Game.h"

class View
{
public:
	
	void Initialize();
	void Draw(const Game& GameInstance);
	
	void SwapBuffers(HDC DC);
	
	void OnWindowChanged(HWND Window);

private:
	void DrawBackground(float DangerLevel);

	void DrawCircle(HDC DC, Vector2 Pos, int Radius, HBRUSH Brush);
	void DrawTank(HDC DC, Vector2 Pos, float Angle, HBRUSH Brush);


	HDC BufferDC = nullptr;
	HBITMAP BufferBitmap = nullptr;
	int BufferWidth;
	int BufferHeight;
	
	unsigned int* BackgroundBytes = nullptr;


	HBRUSH Brush_Background;
	HBRUSH Brush_Player;
	HBRUSH Brush_Enemy;
	HBRUSH Brush_EnemyOverlapping;
	HBRUSH Brush_EnemyInRange;
	HBRUSH Brush_EnemyBullet;
	HBRUSH Brush_TankBullet;

	HBRUSH Brush_Black;
	HBRUSH Brush_White;
	HPEN Pen_Tank_Gun;	
	HPEN Pen_Click_Indicator;
};