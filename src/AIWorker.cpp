#include "AIWorker.h"
#include "Game.h"

DWORD WINAPI ThreadFunc(void* Param);

void AIWorker::Initialize(Game* InGameInstance)
{
	GameInstance = InGameInstance;

	HANDLE Thread = CreateThread(nullptr, 0, ThreadFunc, this, 0, nullptr);	
	SetThreadPriority(Thread, THREAD_PRIORITY_LOWEST);
}

void AIWorker::Shutdown()
{
	Quit = true;
}

void AIWorker::RunThread()
{
	while (!Quit)
	{
		Vector2 PlayerPos = GameInstance->Player.Pos;

		for (int i = 0; i < GameInstance->NumEnemies; i++)
		{
			// Lock mutex since we're accessing enemies data on thread
			GameInstance->LockMutex();

			if (GameInstance->Enemies[i].InRange)
			{
				// Calculate angle from enemy to player

				Vector2 EnemyPos = GameInstance->Enemies[i].Pos;
			
				Vector2 ToPlayer = PlayerPos - EnemyPos;
				ToPlayer.Normalize();

				GameInstance->Enemies[i].Angle = (float)ToPlayer.Angle();
			}

			// Release data mutex
			GameInstance->ReleaseMutex();
		}

		AIThread_FrameCount++;
	}
}

DWORD WINAPI ThreadFunc(void* Param) 
{
	AIWorker* Instance = static_cast<AIWorker*>(Param);
	if (Instance != nullptr)
	{
		Instance->RunThread();
	}	
	return 0;
}