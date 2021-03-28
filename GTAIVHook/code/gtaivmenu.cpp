#include "gtaivmenu.h"
#include <iostream>
#include "..\imgui\imgui.h"
#include "gtaivutils.h"
#include "gtaiv.h"
#include <chrono>
GTAIVMenu* TheMenu = new GTAIVMenu();



const char* szWeapons[] = {
	"BASEBALLBAT",
	"POOLCUE",
	"KNIFE",
	"GRENADE",
	"MOLOTOV",
	"ROCKET",
	"PISTOL",
	"DEAGLE",
	"SHOTGUN",
	"BARETTA",
	"MICRO_UZI",
	"MP5",
	"AK47",
	"M4",
	"SNIPERRIFLE",
	"M40A1",
	"RLAUNCHER"

};


// copied from vehicles.ide
const char* szVehicles[] = { "admiral",	"airtug","ambulance","banshee",	"benson", "biff","blista",
	"bobcat","boxville", "buccaneer","burrito", "burrito2","bus","cabby", "cavalcade","chavos", "cognoscenti","comet",
	"coquette","df8","dilettante","dukes","e109","emperor","emperor2","esperanto","faction","fbi","feltzer","feroci","feroci2",
	"firetruk","flatbed","fortune","forklift","futo","fxt","habanero","hakumai","huntley","infernus","ingot","intruder","landstalker",
	"lokus","manana","marbella","merit","minivan","moonbeam","mrtasty","mule","noose","nstockade","oracle","packer","patriot",
	"perennial","perennial2","peyote","phantom","pinnacle","pmp600","police","police2","polpatriot","pony","premier","pres","primo","pstockade",
	"rancher","rebla","ripley","romero","rom","ruiner","sabre","sabre2","sabregt","schafter","sentinel","solair","speedo","stalion","steed",
	"stockade","stratum","stretch","sultan","sultanrs","supergt","taxi","taxi2","trash","turismo","uranus","vigero","vigero2","vincent","virgo",
	"voodoo","washington","willard","yankee","bobber","faggio","hellfury","nrg900","pcj","sanchez","zombieb","annihilator","maverick","polmav","tourmav",
	"dinghy","jetmax","marquis","predator","reefer","squalo","tuga","tropic"
};

void GTAIVMenu::Initialize()
{
	bIsActive = false;
	bFocused = false;
	playerPos = { 0,0,0 };
	carPos = { 0,0,0 };
	teleportPos = { 0,0,0 };
	playerCharHandle = 0;
	spawnedCarHandle = 0;
	bUpdatePos = true;
	wheelStatus = 0;
	bUseManualName = false;
	bUseManualNameCar = false;
	sprintf(szSelectedWeapon, szWeapons[0]);
	sprintf(szSelectedCar, szVehicles[0]);

}

void GTAIVMenu::Draw()
{
	ImGui::GetIO().MouseDrawCursor = true;
	ImGui::Begin(GetGTAIVHookVersion());
	if (GTAIV::getPlayer())
	{
		if (ImGui::BeginTabBar("##tabs"))
		{

			if (ImGui::BeginTabItem("Weapons"))
			{
				ImGui::Checkbox("Use Name for Spawning", &bUseManualName);
				if (!bUseManualName)
				{
					if (ImGui::BeginCombo("Select Weapon", szSelectedWeapon))
					{
						for (int n = 0; n < IM_ARRAYSIZE(szWeapons); n++)
						{
							bool is_selected = (szSelectedWeapon == szWeapons[n]);
							if (ImGui::Selectable(szWeapons[n], is_selected))
								sprintf(szSelectedWeapon, szWeapons[n]);
							if (is_selected)
								ImGui::SetItemDefaultFocus();

						}
						ImGui::EndCombo();
					}
				}
				else
				{
					ImGui::InputText("Type Weapon Name", szSelectedWeapon, sizeof(szSelectedWeapon));
				}


				if (ImGui::Button("Give Weapon To Player"))
				{
					SendMenuMessage(MENU_GIVE_WEAPON);

				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Player"))
			{
				GTAIV_Natives::GetPlayerChar(0, &playerCharHandle);
				GTAIV_Natives::GetCharCoordinates(playerCharHandle, &playerPos.x, &playerPos.y, &playerPos.z);
				ImGui::InputFloat3("Position", &playerPos.x);

				ImGui::Separator();

				ImGui::Text("Teleport");
				ImGui::InputFloat3("X | Y | Z", &teleportPos.x);

				if (ImGui::Button("Warp"))
					SendMenuMessage(MENU_TELEPORT_PLAYER);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Vehicles"))
			{
				ImGui::Checkbox("Use Name for Spawning ", &bUseManualNameCar);

				if (!bUseManualNameCar)
				{
					if (ImGui::BeginCombo("Select Vehicle", szSelectedCar))
					{
						for (int n = 0; n < IM_ARRAYSIZE(szVehicles); n++)
						{
							bool is_selected = (szSelectedCar == szVehicles[n]);
							if (ImGui::Selectable(szVehicles[n], is_selected))
								sprintf(szSelectedCar, szVehicles[n]);
							if (is_selected)
								ImGui::SetItemDefaultFocus();

						}
						ImGui::EndCombo();
					}
				}
				else
				{
					ImGui::InputText("Type Vehicle Name", szSelectedCar, sizeof(szSelectedCar));
				}


				if (ImGui::Button("Create"))
				{
					SendMenuMessage(MENU_CREATE_CAR);
				}
				ImGui::EndTabItem();
			}


			ImGui::EndTabBar();
		}
	}
	else
		ImGui::Text("Menu will appear once ingame!");

}

void GTAIVMenu::Process()
{
	switch (menuMessage)
	{
	case MENU_CREATE_CAR:
		unsigned int model;
		int  id;
		float x;
		float y;
		float z;


		if (spawnedCarHandle)
			GTAIV_Natives::DeleteCar(&spawnedCarHandle);

		model = GTAIV::hashString(szSelectedCar);
		GTAIV::getModelByKey(model, &id);


		if (id > 0)
		{
			GTAIV::RequestModel(model);

			if (GTAIV_Natives::HasModelLoaded(model))
			{
				GTAIV_Natives::GetPlayerChar(0, &playerCharHandle);
				GTAIV_Natives::GetCharCoordinates(playerCharHandle, &x, &y, &z);
				GTAIV_Natives::CreateCar(model, x - 2, y, z, &spawnedCarHandle, true);
				GTAIV_Natives::SetCarOnGroundProperly(spawnedCarHandle);

				GTAIV::getModelByKey(model, &id);

				sprintf(msgTemp, "Spawned %s (ID - %d)", szSelectedCar, id);
				GTAIV_Natives::PrintStringNow(msgTemp, 4000);
				GTAIV::SetModelNoLongerNeeded(model);
			}
			else
			{
				sprintf(msgTemp, "Model %s loaded, now you can spawn.", szSelectedCar);
				GTAIV_Natives::PrintStringNow(msgTemp, 4000);
			}
		}
		else
		{
			sprintf(msgTemp, "Invalid vehicle - %s", szSelectedCar);
			GTAIV_Natives::PrintStringNow(msgTemp, 4000);
		}




		menuMessage = 0;
		break;
	case MENU_GIVE_WEAPON:
		unsigned int weaponHash;
		int			 weaponID;

		weaponHash = GTAIV::hashString(szSelectedWeapon);


		GTAIV_Natives::GetPlayerChar(0, &playerCharHandle);
		weaponID = GTAIV::getWeaponIDFromHash(weaponHash, -1);
		if (weaponID >= 0)
		{
			GTAIV_Natives::GiveWeaponToChar(playerCharHandle, weaponID, 500, 1);
			sprintf(msgTemp, "Created %s (ID - %d)", szSelectedWeapon, weaponID);
			GTAIV_Natives::PrintStringNow(msgTemp, 4000);
		}
		else
		{
			sprintf(msgTemp, "Weapon %s doesn't exist!", szSelectedWeapon);
			GTAIV_Natives::PrintStringNow(msgTemp, 4000);
		}

		menuMessage = 0;
		break;
	case MENU_TELEPORT_PLAYER:
		GTAIV_Natives::GetPlayerChar(0, &playerCharHandle);
		GTAIV_Natives::SetCharCoordinates(playerCharHandle, teleportPos.x, teleportPos.y, teleportPos.z);

		menuMessage = 0;
		break;
	default:
		menuMessage = 0;
		break;
	}
}

bool GTAIVMenu::GetActiveState()
{
	return bIsActive;
}

void GTAIVMenu::MouseWorkaround()
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = 0;
	io.MouseDown[1] = 0;
	io.MouseDown[2] = 0;
	bUpdatePos = false;
}

void GTAIVMenu::SendMenuMessage(eMenuMessage msg)
{
	menuMessage = msg;
}

char * GetGTAIVHookVersion()
{
	char buffer[512];
	sprintf(buffer, "GTAIVHook by ermaccer (%s)", GTAIVHOOK_VERSION);
	return buffer;
}

bool TimeWait(float time)
{
}
