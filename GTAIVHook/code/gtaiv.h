#pragma once
#include <d3d9.h>

#define  GTAIVHOOK_VERSION "test"


struct CVector {
	float x, y, z;
};


namespace GTAIV {
	int 		 getPlayer();
	int			 getWeaponIDFromHash(unsigned int hash, int alternative);
	unsigned int hashString(char* string);


	void		RequestModel(unsigned int hash);
	void		SetModelNoLongerNeeded(unsigned int hash);
	void		ProcessInput(bool a1);
	void		getModelByKey(unsigned int hash, int* model);
}


namespace GTAIV_Natives {
	void GetMouseWheel(int* val);


	// char
	void SetCharCoordinates(int chr, float x, float y, float z);
	void GetPlayerChar(int index, int* ptr);
	void GetCharCoordinates(int chr, float* x, float* y, float* z);
	void GiveWeaponToChar(int chr, int id, int ammo, int show);
	void FreezeCharPosition(int chr, int status);

	// hud
	void PrintStringNow(char* msg, int time);

	// streaming
	void		RequestModel(unsigned int hash);
	bool		HasModelLoaded(unsigned int hash);


	// cars
	void	GetRandomCarModelInMemory(bool unk, unsigned int* hash, int* unk2);
	void	CreateCar(unsigned int hash, float x, float y, float z, int* ptr, bool unk);
	void    SetCarOnGroundProperly(int handle);
	void    DeleteCar(int* handle);
}




