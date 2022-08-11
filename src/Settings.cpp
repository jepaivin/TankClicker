#define _CRT_SECURE_NO_WARNINGS

#include "framework.h"
#include "Settings.h"


void Settings::Load()
{
	const char* Keys[] = { "PlayerSpeed", "PlayerMaxRange", "EnemySpeed", "BulletSpeed", "EnemyBulletSpeed", "NumEnemies"};
	int* Params[] = { &PlayerSpeed, &PlayerMaxRange, &EnemySpeed, &BulletSpeed, &EnemyBulletSpeed, &NumEnemies };

	FILE* File = fopen("settings.txt", "r");
	if (File)
	{
		char Line[256];
		while (fgets(Line, 256, File) != nullptr)
		{
			char* Key = strtok(Line, " ");
			char* Value = strtok(nullptr, " ");
			int NumericValue = atoi(Value);

			for (int i = 0; i < sizeof(Keys)/sizeof(const char*); i++)
			{
				if (_stricmp(Key, Keys[i]) == 0)
				{
					*Params[i] = NumericValue;
				}
			}
		}
	}
	fclose(File);
}

Settings GetSettings()
{
	Settings Instance;
	Instance.Load();
	return Instance;
}
