// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LobbyGameModeBase.h"

#include "Controller/LobbyPlayerController.h"
#include "EngineUtils.h"
#include "Game/LobbyGameStateBase.h"
#include "PlayerState/LobbyPlayerState.h"

ALobbyGameModeBase::ALobbyGameModeBase()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(NewPlayer))
	{
		LobbyPlayerControllers.Add(LobbyPC);
	}
}

void ALobbyGameModeBase::JoinPlayerInLobby(ALobbyPlayerController* LobbyPC)
{
	if (ALobbyGameStateBase* LobbyGS = GetGameState<ALobbyGameStateBase>())
	{
		if (ALobbyPlayerState* LobbyPS = LobbyPC->GetPlayerState<ALobbyPlayerState>())
		{
			if (LobbyPS->PlayerInfo.PlayerName.IsEmpty())
			{
				FPlayerInfo Infos;
				Infos.PlayerName = TEXT("Player") + FString::FromInt(LobbyPlayerControllers.Num());
				Infos.PlayerIndex = LobbyPlayerControllers.Num();
				Infos.PlayerJob = EPlayerJob::None;
				Infos.bIsArrest = false;

				LobbyPS->PlayerInfo = Infos;
				LobbyGS->PlayerInfos.Add(LobbyPS->PlayerInfo);
			}
		}
	}
}

void ALobbyGameModeBase::FirstSyncLobbyList()
{
	if (ALobbyGameStateBase* LobbyGS = GetGameState<ALobbyGameStateBase>())
	{
		LobbyGS->OnRep_PlayerInfos();
	}
	int32 PlayerIndex = 1;
	for (TActorIterator<ALobbyPlayerController> It(GetWorld()); It; ++It)
	{
		ALobbyPlayerController* LobbyPC = *It;
		if (IsValid(LobbyPC))
		{
			if (ALobbyPlayerState* LobbyPS = LobbyPC->GetPlayerState<ALobbyPlayerState>())
			{
				if (LobbyPS->PlayerInfo.PlayerName.IsEmpty())
				{
					FPlayerInfo NewInfo = LobbyPS->PlayerInfo;
					NewInfo.PlayerIndex = PlayerIndex;
					LobbyPS->PlayerInfo = NewInfo;
					PlayerIndex++;
				}
			}
		}
	}

}

void ALobbyGameModeBase::StartGameServerTravel()
{
	if (LobbyPlayerControllers.Num() >= 2) // 시작 인원 2명 이상일 때만 가능
	{
		PreGameStartJobSetting();

		FString MapName = TEXT("/Game/ATB/Maps/MainLevel");
		FString URL = MapName + TEXT("?listen");
		bool bIsSuccess = GetWorld()->ServerTravel(URL, true);

		if (bIsSuccess)
		{
			if (ALobbyGameStateBase* LobbyGS = GetGameState<ALobbyGameStateBase>())
			{
				LobbyGS->MulticastRPCSuccessTravel();
			}
		}
	}
}

void ALobbyGameModeBase::ExitGame(int32 Index)
{
	if (LobbyPlayerControllers.IsValidIndex(Index - 1))
	{
		LobbyPlayerControllers[Index - 1]->ClientRPCExitGame();
		LobbyPlayerControllers.RemoveAt(Index - 1);
	}

	ALobbyGameStateBase* LobbyGS = GetGameState<ALobbyGameStateBase>();

	if (IsValid(LobbyGS))
	{
		for (int32 i = 0; i < LobbyGS->PlayerInfos.Num(); ++i)
		{
			if (LobbyGS->PlayerInfos[i].PlayerIndex == Index)
			{			
				LobbyGS->PlayerInfos.RemoveAt(i);
				break;
			}
		}
	}
	ReIndexingPlayerInfos();
}

void ALobbyGameModeBase::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ThisClass::SettingPlayers, 0.5f, false);
}

void ALobbyGameModeBase::SettingPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(*It))
		{
			LobbyPlayerControllers.Add(LobbyPC);
			if (ALobbyGameStateBase* LobbyGS = GetGameState<ALobbyGameStateBase>())
			{
				if (ALobbyPlayerState* LobbyPS = LobbyPC->GetPlayerState<ALobbyPlayerState>())
				{
					LobbyGS->PlayerInfos.Add(LobbyPS->PlayerInfo);
				}
			}
		}
	}
	PostSeamlessTravelPlayerInfos();
}

void ALobbyGameModeBase::PostSeamlessTravelPlayerInfos()
{
	ALobbyGameStateBase* LobbyGS = GetGameState<ALobbyGameStateBase>();
	if (IsValid(LobbyGS))
	{
		for (int32 i = 0; i < LobbyGS->PlayerInfos.Num(); ++i)
		{
			if (ALobbyPlayerState* LobbyPS = LobbyPlayerControllers[i]->GetPlayerState<ALobbyPlayerState>())
			{
				LobbyPS->ClientSettingForPlayerIndex(); // PlayerIndex 즉 StartButton에 대한 Visible 재세팅
			}
		}
	}
}

void ALobbyGameModeBase::PreGameStartJobSetting()
{
	int32 PoliceCount = LobbyPlayerControllers.Num() >= 5 ? 2 : 1; // 총 인원 수가 5명 이상이라면 경찰 직업 2개

	for (ALobbyPlayerController* LobbyPC : LobbyPlayerControllers)
	{
		if (ALobbyPlayerState* LobbyPS = LobbyPC->GetPlayerState<ALobbyPlayerState>())
		{
			if (PoliceCount != 0)
			{
				int32 Job = RandomJob(); //랜덤한 직업 설정
				if (Job == 1) // 1은 경찰
				{
					LobbyPS->PlayerInfo.PlayerJob = EPlayerJob::Police;
					--PoliceCount;
				}
				else // 0은 도둑
				{
					LobbyPS->PlayerInfo.PlayerJob = EPlayerJob::Theif;
				}
			}
			else
			{
				LobbyPS->PlayerInfo.PlayerJob = EPlayerJob::Theif;
			}
		}
	}


	for (int32 i = 0; i < LobbyPlayerControllers.Num(); ++i)
	{
		if (ALobbyPlayerState* LobbyPS = LobbyPlayerControllers[i]->GetPlayerState<ALobbyPlayerState>())
		{
			if (i == LobbyPlayerControllers.Num() - 1) // 마지막 인덱스에도 경찰이 다 나오지 않았다면? 바로 경찰
			{
				if (PoliceCount != 0)
				{
					LobbyPS->PlayerInfo.PlayerJob = EPlayerJob::Police;
				}
			}
			else 
			{
				if (PoliceCount != 0)
				{
					int32 Job = RandomJob(); //랜덤한 직업 설정
					if (Job == 1) // 1은 경찰
					{
						LobbyPS->PlayerInfo.PlayerJob = EPlayerJob::Police;
						--PoliceCount;
					}
					else // 0은 도둑
					{
						LobbyPS->PlayerInfo.PlayerJob = EPlayerJob::Theif;
					}
				}
			}
		}
	}

}

int32 ALobbyGameModeBase::RandomJob()
{
	return FMath::RandRange(0, 1);
}

void ALobbyGameModeBase::ReIndexingPlayerInfos()
{
	ALobbyGameStateBase* LobbyGS = GetGameState<ALobbyGameStateBase>();

	if (IsValid(LobbyGS))
	{
		for (int32 i = 0; i < LobbyGS->PlayerInfos.Num(); ++i)
		{
			LobbyGS->PlayerInfos[i].PlayerIndex = i + 1;
			if (LobbyPlayerControllers.IsValidIndex(i))
			{
				if (ALobbyPlayerState* LobbyPS = LobbyPlayerControllers[i]->GetPlayerState<ALobbyPlayerState>())
				{
					FPlayerInfo NewInfo = LobbyPS->PlayerInfo;
					NewInfo.PlayerIndex = i + 1;
					LobbyPS->PlayerInfo = NewInfo;
				}
			}
		}
	}
}
