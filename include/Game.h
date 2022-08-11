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

	Vector2 GetEnemyPosition(int EnemyIndex);	

	// Calculate a "danger level" for UI based on how many enemies are within range
	double GetDangerLevel() const;
	
	double GetDistanceToNearestEnemy() const;
	
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