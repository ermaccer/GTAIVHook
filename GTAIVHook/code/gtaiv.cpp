#include "gtaiv.h"
#include "gtaivutils.h"
#include <iostream>
#include "gtaivmenu.h"
#include "..\pattern\Hooking.Patterns.h"

int GTAIV::getPlayer()
{
	return *(int*)_ivaddr(0x11A8808);
}

int GTAIV::getWeaponIDFromHash(unsigned int hash, int alternative)
{
	return 	((int(__cdecl*)(unsigned int, int))_ivaddr(0xAF5C10))(hash,alternative);
}

unsigned int GTAIV::hashString(char * string)
{
	return 	((unsigned int(__cdecl*)(char*))_ivaddr(0x8D4980))(string);
}

void GTAIV::RequestModel(unsigned int hash)
{
	int thread = ((int(__cdecl*)())_ivaddr(0x94B400))();
	((void(__cdecl*)(unsigned int, int))_ivaddr(0xBB75F0))(hash, thread);
}

void GTAIV::SetModelNoLongerNeeded(unsigned int hash)
{
	int thread = ((int(__cdecl*)())_ivaddr(0x94B400))();
	((void(__cdecl*)(unsigned int, int))_ivaddr(0xBB72C0))(hash, thread);
}

void GTAIV::ProcessInput(bool a1)
{
	if (TheMenu->bIsActive)
		return;
	((void(__cdecl*)(int))_ivaddr(0x69F0C0))(a1);
}


void GTAIV::getModelByKey(unsigned int hash, int * model)
{
	((int(__cdecl*)(unsigned int, int*))_ivaddr(0x9DBFF0))(hash, model);
}


void GTAIV_Natives::SetCharCoordinates(int chr, float x, float y, float z)
{
	((void(__cdecl*)(int, float, float, float))_ivaddr(0xBAAEB0))(chr, x, y,z);
}

void GTAIV_Natives::GetPlayerChar(int index, int* ptr)
{
	((void(__cdecl*)(int, int*))_ivaddr(0xBB3050))(index, ptr);
}




void GTAIV_Natives::GetCharCoordinates(int chr, float * x, float * y, float * z)
{
	((void(__cdecl*)(int, float*,float*,float*))_ivaddr(0xBA6000))(chr,x,y,z);
}

void GTAIV_Natives::GiveWeaponToChar(int chr, int id, int ammo, int show)
{
	((void(__cdecl*)(int, int,int,int))_ivaddr(0xBD0F10))(chr, id, ammo, show);
}

void GTAIV_Natives::FreezeCharPosition(int chr, int status)
{
	((void(__cdecl*)(int, int))_ivaddr(0xBA5AE0))(chr,status);
}

void GTAIV_Natives::GetMouseWheel(int * val)
{
	((void(__cdecl*)(int*))_ivaddr(0xB98270))(val);
}

void GTAIV_Natives::PrintStringNow(char * msg, int time)
{
	((void(__cdecl*)(char*, char*, int, bool))_ivaddr(0xB902A0))("STRING",msg,time,true);
}


void GTAIV_Natives::RequestModel(unsigned int hash)
{
	((void(__cdecl*)(unsigned int))_ivaddr(0xBB70E0))(hash);
}

bool GTAIV_Natives::HasModelLoaded(unsigned int hash)
{
	return ((bool(__cdecl*)(unsigned int))_ivaddr(0xBB71A0))(hash);
}

void GTAIV_Natives::GetRandomCarModelInMemory(bool unk, unsigned int * hash, int * unk2)
{
	((void(__cdecl*)(bool, unsigned int*, int*))_ivaddr(0xBCAF40))(unk,hash,unk2);
}

void GTAIV_Natives::CreateCar(unsigned int hash, float x, float y, float z, int* ptr, bool unk)
{
	((void(__cdecl*)(unsigned int, float, float, float, int*, bool))_ivaddr(0xBC90B0))(hash,x,y,z,ptr,unk);
}

void GTAIV_Natives::SetCarOnGroundProperly(int handle)
{
	((void(__cdecl*)(int))_ivaddr(0xBCD640))(handle);
}

void GTAIV_Natives::DeleteCar(int* handle)
{
	((void(__cdecl*)(int*))_ivaddr(0xBC96F0))(handle);
}
