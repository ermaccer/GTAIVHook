#pragma once
#include "gtaiv.h"
#include <Windows.h>
// as usual, based on mh2 debug menu


enum eMenuMessage {
	MENU_CREATE_CAR = 1,
	MENU_GIVE_WEAPON,
	MENU_TELEPORT_PLAYER
};

class GTAIVMenu {
private:

public:
	bool bIsActive;
	int  menuMessage;
	// window data
	bool bFocused;
	int  wheelStatus;
	bool bUpdatePos;


	char szSelectedWeapon[128] = {};
	char szSelectedCar[128] = {};
	char msgTemp[512] = {};
	bool bUseManualName;
	bool bUseManualNameCar;

	CVector playerPos;
	CVector teleportPos;
	int		playerCharHandle;




	CVector carPos;
	int     spawnedCarHandle;

	void Initialize();
	void Draw();
	void Process();
	bool GetActiveState();
	void MouseWorkaround();

	void SendMenuMessage(eMenuMessage msg);
};

char* GetGTAIVHookVersion();
extern GTAIVMenu* TheMenu;

