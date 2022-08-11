#pragma once

class Settings
{
public:
	int PlayerSpeed = 0;
	int PlayerMaxRange = 0;
	int EnemySpeed = 0;
	int BulletSpeed = 0;
	int EnemyBulletSpeed = 0;
	int NumEnemies = 0;
	
	void Load();
};


extern Settings GetSettings();