#include "framework.h"
#include "View.h"
#include "Game.h"
#include "Settings.h"

int MainFrameCount = 0;
int AIWorkerFrameCount = 0;


void Game::Initialize()
{
	NumEnemies = GetSettings().NumEnemies;
	
	Enemies = new TankData[NumEnemies];
	
	for (int i = 0; i < NumEnemies; i++)
	{
		Enemies[i].Pos.X = (float)rand() / RAND_MAX * MAP_WIDTH;
		Enemies[i].Pos.Y = (float)rand() / RAND_MAX * MAP_HEIGHT;		
	}

	Player.Pos.X = 500;
	Player.Pos.Y = 500;


	AIWorker.Initialize(this);
	Mutex = CreateMutex(nullptr, false, L"GameDataMutex");
}

void Game::Shutdown()
{
	AIWorker.Shutdown();

	CloseHandle(Mutex);
}


void Game::UpdateEnemiesInRange()
{
	LockMutex();

	double MaxRange = GetSettings().PlayerMaxRange;

	for (int i = 0; i < NumEnemies; i++)
	{
		double Distance = (GetEnemyPosition(i) - Player.Pos).Length();
		
		Enemies[i].InRange = Distance < MaxRange;
	}
	ReleaseMutex();

}

void Game::Tick(float DeltaTime)
{
	// Move player towards target

	Vector2 ToTarget = MoveTarget - Player.Pos;	
	ToTarget.Normalize();

	ToTarget = ToTarget * DeltaTime * GetSettings().PlayerSpeed;

	Player.Pos = Player.Pos + ToTarget;	
	
	// Update "in range" status of enemies
	UpdateEnemiesInRange();
}

void Game::OnMouseDown(POINT Point, EMouseButton Button)
{	
	MoveTarget.X = Point.x;
	MoveTarget.Y = Point.y;

	Vector2 ToTarget = MoveTarget - Player.Pos;
	ToTarget.Normalize();

	Player.Angle = (float)ToTarget.Angle();
}

void Game::OnMouseUp(POINT Point, EMouseButton Button)
{

}

void Game::LockMutex()
{
	// Lock the mutex protecting game state data
	WaitForSingleObject(Mutex, INFINITE);
}

void Game::ReleaseMutex()
{
	// Release the mutex protecting game state data
	::ReleaseMutex(Mutex);
}