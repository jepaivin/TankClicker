#pragma once
#include "framework.h"


class AIWorker
{
public:
	void Initialize(class Game* GameInstance);
	void Shutdown();

	void RunThread();

private:
	
	bool Quit = false;
	
	Game* GameInstance = nullptr;

};