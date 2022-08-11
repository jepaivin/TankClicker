#pragma once
#include <windows.h>
#include "AIWorker.h"
#include "Definitions.h"

class Game
{
public:
	void Initialize();
	void Shutdown();
	
	void Tick(float DeltaTime);

	void Draw(HDC DC);

	void OnMouseDown(POINT Point, EMouseButton Button);
	void OnMouseUp(POINT Point, EMouseButton Button);	

	Vector2 GetEnemyPosition(int EnemyIndex)
	{
		for (int i = 0; i < NumEnemies; i++)
		{
			if (i == EnemyIndex)
			{
				return Enemies[i].Pos;
			}
		}
		return Vector2();
	}
	
	void UpdateEnemiesInRange();

	// Thread synchronization

	void LockMutex();
	void ReleaseMutex();
	
	int NumEnemies;
	TankData* Enemies;
	TankData Player;	
	Vector2 MoveTarget;
	
	AIWorker AIWorker;

	HANDLE Mutex;
};