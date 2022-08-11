#pragma once

#include <math.h>

enum EMouseButton
{
	Left,
	Middle,
	Right
};

#define MAP_WIDTH 8192
#define MAP_HEIGHT 4096

extern int MainThread_FrameCount;
extern int AIThread_FrameCount;
extern int MainThread_FPS;
extern int AIThread_FPS;

struct Vector2
{
	double X;
	double Y;

	Vector2() : X(0), Y(0)
	{

	}

	Vector2 operator + (const Vector2& Other)
	{
		Vector2 Result;
		Result.X = X + Other.X;
		Result.Y = Y + Other.Y;
		return Result;
	}
	Vector2 operator - (const Vector2& Other)
	{
		Vector2 Result;
		Result.X = X - Other.X;
		Result.Y = Y - Other.Y;
		return Result;
	}
	Vector2 operator * (double Scale)
	{
		Vector2 Result;
		Result.X = X * Scale;
		Result.Y = Y * Scale;
		return Result;
	}


	double Length() const
	{
		return sqrt(X * X + Y * Y);
	}

	double LengthSq() const
	{
		return X * X + Y * Y;
	}

	void Normalize()
	{
		double Len = Length();
		if (Len > 0.0f)
		{
			X = X / Len;
			Y = Y / Len;
		}
	}

	double Angle()
	{
		return atan2(Y, X);
	}
};

struct TankData
{
	Vector2 Pos;
	float Angle;
	float DistanceToPlayer;
	bool InRange;
};
