#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <detours.h>
#include "offsets.h"
#include "../ue4.h"
#include "../memory.h"
#include "util.h"
#include "../skCrypter.h"

namespace ChapterOneCore
{
	UE4::GObjects* GlobalObjects;
	UE4::UObject** GWorld;

	UE4::UObject* CheatManager;
	UE4::UObject* GameMode;
	UE4::UObject* Controller;
	UE4::UObject* Pawn;
	UE4::UObject* GameState;

	bool bInFrontend;
	bool bIsLoading;
	bool bIsInGame;
	bool bHasAlreadyTriggeredEvent;
	bool bEnableKeybinds;

	namespace Offsets
	{
		UE4::UObject* JumpFunc;

		DWORD GamePhase;
	}

	void GetOffsets()
	{
		Offsets::JumpFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.Character.Jump"));
		Offsets::GamePhase = GlobalObjects->FindOffsetOld(skCrypt("EnumProperty /Script/FortniteGame.FortGameStateAthena.GamePhase"));
	}

	/*static void ExecutePatches()
	{
#pragma warning( push )
#pragma warning( disable : 4309 )
		if (UE4::SprintPatchAddr)
		{
			*(char*)(UE4::SprintPatchAddr + 0x0) = 0x90;
			*(char*)(UE4::SprintPatchAddr + 0x1) = 0x90;
			*(char*)(UE4::SprintPatchAddr + 0x2) = 0x90;
			*(char*)(UE4::SprintPatchAddr + 0x3) = 0x90;
			*(char*)(UE4::SprintPatchAddr + 0x4) = 0x90;
			*(char*)(UE4::SprintPatchAddr + 0x5) = 0x90;
		}
#pragma warning( pop )
	}*/
	class RiftChapterOneUtils
	{
	public:
		static UE4::UObject* GetPawnFromController(UE4::UObject* InController)
		{
			DWORD PawnOffset = GlobalObjects->FindOffsetOld(skCrypt("ObjectProperty /Script/Engine.Controller.Pawn"));
			return *reinterpret_cast<UE4::UObject**>(__int64(InController), __int64(PawnOffset));
		}

		static UE4::UObject* GetPlayerStateFromController(UE4::UObject* InController)
		{
			DWORD PlayerStateOffset = GlobalObjects->FindOffsetOld(skCrypt("ObjectProperty /Script/Engine.Controller.PlayerState"));
			auto ReturnValue = ReadPointer(InController, PlayerStateOffset);
			return reinterpret_cast<UE4::UObject*>(ReturnValue);
		}

		static std::string GetGameVersion()
		{
			auto GetGameVersionFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortRuntimeOptions.GetGameVersion"));
			auto Default__FortRuntimeOptionsObj = GlobalObjects->FindObjectByFullName(skCrypt("FortRuntimeOptions /Script/FortniteGame.Default__FortRuntimeOptions"));
			auto GetRuntimeOptionsFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortRuntimeOptions.GetRuntimeOptions"));
			UE4::UObject* RuntimeOptionsReturnValue;
			UE4::FString VersionReturnValue;

			UE4::ProcessEvent(Default__FortRuntimeOptionsObj, GetRuntimeOptionsFunc, &RuntimeOptionsReturnValue, 0);

			UE4::ProcessEvent(RuntimeOptionsReturnValue, GetGameVersionFunc, &VersionReturnValue, 0);

			return VersionReturnValue.ToString();
		}

		static void DestroyAll(UE4::UObject* InCheatManager, const char* ClassToDestroy)
		{
			UE4::UObject* LocatedClass = GlobalObjects->FindObject(ClassToDestroy);
			UE4::ProcessEvent(InCheatManager, GlobalObjects->FindObjectByFullName("Function /Script/Engine.CheatManager.DestroyAll"), &LocatedClass, 0);
		}

		enum class EAthenaGamePhase : uint8_t
		{
			None = 0,
			Setup = 1,
			Warmup = 2,
			Aircraft = 3,
			SafeZones = 4,
			EndGame = 5,
			Count = 6,
			EAthenaGamePhase_MAX = 7
		};

		static void OnRep_GamePhase(UE4::UObject* State, EAthenaGamePhase LastPhase)
		{
			static UE4::UObject* OnRep_GamePhase = GlobalObjects->FindObject(skCrypt("Function /Script/FortniteGame.FortGameStateAthena.OnRep_GamePhase"));

			UE4::ProcessEvent(State, OnRep_GamePhase, &LastPhase, 0);
		}

		static void TeleportTo(UE4::UObject* InPawn, float InX, float InY, float InZ)
		{
			struct
			{
				UE4::FVector DestLocation;
				UE4::FRotator DestRotation;
			} TeleportParams;

			TeleportParams.DestLocation = UE4::FVector{ InX, InY, InZ }; // Gotham
			TeleportParams.DestRotation = UE4::FRotator{ 0,0,0 };

			UE4::ProcessEvent(InPawn, GlobalObjects->FindObjectByFullName("Function /Script/Engine.Actor.K2_TeleportTo"), &TeleportParams, 0);
		}

		static void StartMatch(UE4::UObject* InGameMode)
		{
			UE4::ProcessEvent(InGameMode, GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.GameMode.StartMatch")), nullptr, 0);
		}

		static void ServerReadyToStartMatch(UE4::UObject* InController)
		{ 
			UE4::ProcessEvent(InController, GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPlayerController.ServerReadyToStartMatch")), nullptr, 0);
		}

		static void Possess(UE4::UObject* InController, UE4::UObject* InPawn) 
		{ 
			UE4::ProcessEvent(InController, GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.Controller.Possess")), &InPawn, 0);
		}

		static void SwitchLevel(UE4::UObject* InController, UE4::FString InUrl)
		{
			auto func = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.PlayerController.SwitchLevel"));
			UE4::ProcessEvent(InController, func, &InUrl, 0);
		}

		static UE4::UObject* CreateCheatManager(UE4::UObject* InController)
		{
			UE4::UObject* NewCheatManager = UE4::StaticConstructObject_Internal(GlobalObjects->FindObjectByFullName(skCrypt("Class /Script/Engine.CheatManager")), InController, 0, 0, 0, 0, 0, 0, 0);
			UE4::UObject** CheatManager = reinterpret_cast<UE4::UObject**>(__int64(InController) + __int64(GlobalObjects->FindOffsetOld(skCrypt("ObjectProperty /Script/Engine.PlayerController.CheatManager"))));
			*CheatManager = NewCheatManager;
			return NewCheatManager;
		}

		static UE4::UObject* Summon(UE4::UObject* InCheatManager, const char* ClassToSummon)
		{
			struct
			{
				UE4::FString ClassName;
			} SummonParams;
			std::string StrClassToSummon(ClassToSummon);
			std::wstring WClassToSummon(StrClassToSummon.begin(), StrClassToSummon.end());
			SummonParams.ClassName = WClassToSummon.c_str();
			UE4::ProcessEvent(InCheatManager, GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.CheatManager.Summon")), &SummonParams, 0);
		}

		static void SetDefaultCharacterParts(UE4::UObject* InFortPlayerState)
		{
			UE4::UObject* DefaultHead = GlobalObjects->FindObjectByFullName(skCrypt("CustomCharacterPart /Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1"));
			UE4::UObject* DefaultBody = GlobalObjects->FindObjectByFullName(skCrypt("ustomCharacterPart /Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01"));
			DWORD CharacterPartsOffset = GlobalObjects->FindOffsetOld(skCrypt("StructProperty /Script/FortniteGame.FortPlayerState.CharacterParts"));

			*reinterpret_cast<UE4::UObject**>(__int64(InFortPlayerState) + __int64(CharacterPartsOffset) + __int64(8)) = DefaultHead;
			*reinterpret_cast<UE4::UObject**>(__int64(InFortPlayerState) + __int64(CharacterPartsOffset) + __int64(16)) = DefaultBody;
			UE4::ProcessEvent(InFortPlayerState, GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPlayerState.OnRep_CharacterParts")), nullptr, 0);
		}

		static void SetCurrentMovementStyle(UE4::UObject* InPawn, uint8_t MovementStyle)
		{
			DWORD CurrentMovementStyleOffset = GlobalObjects->FindOffsetOld(skCrypt("ByteProperty /Script/FortniteGame.FortPawn.CurrentMovementStyle"));
			if (CurrentMovementStyleOffset)
			{
				uint8_t* CurrentMovementStyle = reinterpret_cast<uint8_t*>(__int64(InPawn) + __int64(CurrentMovementStyleOffset));
				*CurrentMovementStyle = MovementStyle;
			}
		}
	};

	bool StopHoldingKey = false;
	static DWORD WINAPI RiftPlayerPawnTick(LPVOID lpParam)
	{
		while (true)
		{
			if (bEnableKeybinds)
			{
				if (GetKeyState(VK_SPACE) & 0x8000)
				{
					if (StopHoldingKey == false)
					{
						StopHoldingKey = true;

						UE4::ProcessEvent(Pawn, Offsets::JumpFunc, nullptr, 0);
					}
				}


				else if (GetKeyState(VK_F3) & 0x8000)
				{
					if (StopHoldingKey == false)
					{
						StopHoldingKey = true;
						auto func = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.PlayerController.SwitchLevel"));
						if (func)
						{
							bInFrontend = true;
							bIsLoading = false;
							bIsInGame = false;
							bEnableKeybinds = false;
							bHasAlreadyTriggeredEvent = false;

							const char* CharMapName = skCrypt("Frontend");
							std::string StrMapName(CharMapName);
							std::wstring FStrMapName = std::wstring(StrMapName.begin(), StrMapName.end());
							UE4::FString MapName = FStrMapName.c_str();
							UE4::ProcessEvent(UE4::GetFirstPlayerController(*GWorld), func, &MapName, 0);
						}
					}
				}

				else
					StopHoldingKey = false;

			}
			else
			{
				if (GetKeyState(VK_F5) & 0x8000)
				{
				if (StopHoldingKey == false)
				{
					StopHoldingKey = true;

					auto func = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.PlayerController.SwitchLevel"));
					if (func)
					{
						const char* CharMapName = skCrypt("Athena_Terrain");
						std::string StrMapName(CharMapName);
						std::wstring WStrMapName = std::wstring(StrMapName.begin(), StrMapName.end());
						UE4::FString MapName = WStrMapName.c_str();
						UE4::ProcessEvent(UE4::GetFirstPlayerController(*GWorld), func, &MapName, 0);
						bInFrontend = false;
						bIsLoading = true;
					}
				}
				}
			}

			Sleep(1000 / 60);
		}

		return 0;
	}

	static void LoadAthena()
	{
		Sleep(5000);

		std::cout << "1" << std::endl;
		Controller = (UE4::UObject*)UE4::GetFirstPlayerController(*GWorld);



		GameState = GlobalObjects->FindObjectByFullName(skCrypt("Athena_GameState_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameState_C"));
		
		
		
		std::cout << "2" << std::endl;
		CheatManager = RiftChapterOneUtils::CreateCheatManager(Controller);
		std::cout << "3" << std::endl;
		RiftChapterOneUtils::Summon(CheatManager, skCrypt("PlayerPawn_Athena_C"));
		std::cout << "4" << std::endl;
		Pawn = GlobalObjects->FindObject(skCrypt("PlayerPawn_Athena_C_"));
		std::cout << "5" << std::endl;
		GameMode = GlobalObjects->FindObjectByFullName(skCrypt("Athena_GameMode_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameMode_C"));
		std::cout << "6" << std::endl;
		
		RiftChapterOneUtils::Possess(Controller, Pawn);
		std::cout << "7" << std::endl;
		RiftChapterOneUtils::ServerReadyToStartMatch(Controller);
		std::cout << "8" << std::endl;
		//RiftChapterOneUtils::StartMatch(GameMode);


		RiftChapterOneUtils::EAthenaGamePhase* CurrentGamePhase = reinterpret_cast<RiftChapterOneUtils::EAthenaGamePhase*>(__int64(GameState) + __int64(Offsets::GamePhase));
		*CurrentGamePhase = RiftChapterOneUtils::EAthenaGamePhase::Aircraft;
		//RiftChapterOneUtils::OnRep_GamePhase(GameState, RiftChapterOneUtils::EAthenaGamePhase::None);


		RiftChapterOneUtils::StartMatch(GameMode);


		std::cout << "9" << std::endl;
		RiftChapterOneUtils::TeleportTo(Pawn, 41572, -58218, 1000);
		std::cout << "10" << std::endl;
		std::string GameVersion = RiftChapterOneUtils::GetGameVersion();

		std::cout << GameVersion << std::endl;

	
			UE4::UObject* PlayerState = RiftChapterOneUtils::GetPlayerStateFromController(Controller);
			std::cout << "11" << std::endl;
			RiftChapterOneUtils::SetDefaultCharacterParts(PlayerState);
			std::cout << "12" << std::endl;

		RiftChapterOneUtils::DestroyAll(CheatManager, skCrypt("FortHLODSMActor"));
		std::cout << "13" << std::endl;
		//RiftChapterOneUtils::SetCurrentMovementStyle(Pawn, 3);
		std::cout << "14" << std::endl;
		bEnableKeybinds = true;
		std::cout << "15" << std::endl;
	};

	typedef void* (__fastcall* fProcessEvent)(UE4::UObject* Object, UE4::UObject* Function, void* Params);
	static fProcessEvent ProcessEvent;

	static void* ProcessEventHook(UE4::UObject* Object, UE4::UObject* Function, void* Params)
	{
		if (bInFrontend || bIsLoading)
		{
			std::string FuncName = Function->GetName();

			
				if (FuncName.find(skCrypt("BP_PlayButton")) != std::string::npos)
				{
					Controller = (UE4::UObject*)UE4::GetFirstPlayerController(*GWorld);
					RiftChapterOneUtils::SwitchLevel(Controller, L"Athena_Terrain");
				}
			

			
				if (FuncName.find(skCrypt("OnWorldReady")) != std::string::npos)
				{
					bIsLoading = false;
					//ExecutePatches();
					GetOffsets();
					bIsInGame = true;
					LoadAthena();
				}
		}

		return ProcessEvent(Object, Function, Params);
	}

	static bool PostInit()
	{
		CreateThread(0, 0, RiftPlayerPawnTick, 0, 0, 0);

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)ProcessEvent, ProcessEventHook);
		DetourTransactionCommit();
		return true;
	}

	DWORD StartupGWorldCheckLoop(LPVOID lpParam)
	{
		bool bShouldLoop = true;

		while (bShouldLoop)
		{
			if (*GWorld)
			{
				bShouldLoop = false;
				bInFrontend = true;
				PostInit();
			}
			Sleep(200);
		}
		return NULL;
	}

	static void Setup()
	{
		GWorld = reinterpret_cast<UE4::UObject**>(Memory::FindPattern(skCrypt(C1_GWORLD_PATTERN), true, 3));
		UE4::GObjectsAddr = Memory::FindPattern(skCrypt(C1_GOBJECT_PATTERN), true, 10);
		UE4::FreeAddr = Memory::FindPattern(skCrypt(C1_FMEMORYFREE_PATTERN));
		UE4::GetObjNameAddr = Memory::FindPattern(skCrypt(C1_GETOBJNAME_PATTERN));
		UE4::GetFirstPlayerControllerAddr = Memory::FindPattern(skCrypt(C1_GETFIRSTPLAYERCONTROLLER_PATTERN));
		UE4::ProcessEventAddr = Memory::FindPattern(skCrypt(C1_PROCESSEVENT_PATTERN));
		UE4::StaticConstructObject_InternalAddr = Memory::FindPattern(skCrypt(C1_STATICCONSTRUCTOBJECTINTERNAL_PATTERN));
		//UE4::SprintPatchAddr = Memory::FindPattern(skCrypt(C1_SPRINT_PATCH_PATTERN));

		if (!GWorld)
		{
			GWorld = reinterpret_cast<UE4::UObject**>(Memory::FindPattern(skCrypt("48 8B 1D ? ? ? ? 48 85 DB 74 3B 41"), true, 3));
		}
		if (!UE4::GObjectsAddr) //48 8B 05 ? ? ? ? 48 8D 1C C8 81 4B ? ? ? ? ? 49 63 76 30"
		{
			UE4::GObjectsAddr = Memory::FindPattern(skCrypt("48 8B 05 ? ? ? ? 48 8D 1C C8 81 4B ? ? ? ? ? 49 63 76 30"), true, 3);
		}
		if (!UE4::GetFirstPlayerControllerAddr)
		{
			UE4::GetFirstPlayerControllerAddr = Memory::FindPattern(skCrypt("83 B9 ? ? ? ? ? 7E ? 48 8B 89 ? ? ? ? E9"));
		}
		if (!UE4::FreeAddr)
		{
			UE4::FreeAddr = Memory::FindPattern(skCrypt("48 85 C9 74 1D 4C 8B 05 ? ? ? ? 4D 85 C0"));
		}
		if (!UE4::GetObjNameAddr) //"40 53 48 83 EC 20 48 8B C2 48 8B D9 48 85 D2 75 2E"
		{
			UE4::GetObjNameAddr = Memory::FindPattern(skCrypt("40 53 48 83 EC 20 48 8B C2 48 8B D9 48 85 D2 75 2E"));
		}
		if (!UE4::StaticConstructObject_InternalAddr)
		{
			UE4::StaticConstructObject_InternalAddr = Memory::FindPattern(skCrypt("4C 89 44 24 ? 53 55 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ?"));
		}
		if (!UE4::ProcessEventAddr)
		{
			UE4::ProcessEventAddr = Memory::FindPattern(skCrypt("40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A"));
		}

		GlobalObjects = reinterpret_cast<UE4::GObjects*>(UE4::GObjectsAddr);
		ProcessEvent = (fProcessEvent)(UE4::ProcessEventAddr);

		CreateThread(0, 0, StartupGWorldCheckLoop, 0, 0, 0);
	};
}