// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/ATBGameModeBase.h"
#include "Controller/ATBPlayerController.h"
#include "Game/ATBGameStateBase.h"
#include "PlayerState/LobbyPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Game/JobPlayerStart.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "Character/ATBBotCharacter.h"
#include "Status/StatusComponent.h"
#include "Character/ATBPlayerCharacter.h"

AATBGameModeBase::AATBGameModeBase()
{
	ReadyTime = 30.0f;
	PlayTime = 120.0f;
	bIsPoliceWin = false;
}

void AATBGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnBots();
	GetWorldTimerManager().SetTimer(ServerTimer, this, &ThisClass::OnMainTimerElapsed, 1.0f, true);
}

AActor* AATBGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	ALobbyPlayerState* PS = Player->GetPlayerState<ALobbyPlayerState>();
	if (!PS)
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, AJobPlayerStart::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (AJobPlayerStart* Start = Cast<AJobPlayerStart>(Actor))
		{
			if (Start->SpawnJob == PS->PlayerInfo.PlayerJob)
			{
				return Start;
			}
		}
	}
	return Super::ChoosePlayerStart_Implementation(Player);
}

void AATBGameModeBase::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ThisClass::SettingPlayers, 0.5f, false);
}

void AATBGameModeBase::OnMainTimerElapsed()
{
	AATBGameStateBase* ATBGS = GetGameState<AATBGameStateBase>();

	if (IsValid(ATBGS))
	{
		switch (ATBGS->CurMatchType)
		{
		case EMatchType::None:
			GamePlayerReady();
			break;
		case EMatchType::Ready:
			--ReadyTime;
			NotifyRemainTime(ReadyTime);
			UpdateThievesCount();
			NotifyAllPlayers(TEXT("It's waiting time. Get ready for the game!"));
			if (ReadyTime <= 0)
			{
				ATBGS->CurMatchType = EMatchType::Playing;
			}
			break;
		case EMatchType::Playing:
			--PlayTime;
			NotifyRemainTime(PlayTime);
			UpdateThievesCount();
			if (PlayTime <= 0)
			{
				ATBGS->CurMatchType = EMatchType::Ending;
			}
			else if (PlayTime <= 60)
			{
				PoliceBuffAdded();
			}
			break;
		case EMatchType::Ending:
			JudgeGame();
			PlayerAllStop();
			break;
		default:
			break;
		}
	}
}

void AATBGameModeBase::SettingPlayers()
{
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(PS))
		{
			if (APlayerController* PC = LobbyPS->GetPlayerController())
			{
				if (AATBPlayerController* ATBPC = Cast<AATBPlayerController>(PC))
				{
					if (!AllPlayers.Contains(ATBPC))
					{
						AllPlayers.Add(ATBPC);
						MaxPlayers = LobbyPS->MaxPlayer;
						if (LobbyPS->PlayerInfo.PlayerJob == EPlayerJob::Police)
						{
							UE_LOG(LogTemp, Warning, TEXT("Police : %s"), *LobbyPS->PlayerInfo.PlayerName);
							PolicePlayers.Add(ATBPC);
						}
						else if (LobbyPS->PlayerInfo.PlayerJob == EPlayerJob::Theif)
						{
							
							ThiefPlayers.Add(ATBPC);
						}
					}
				}
			}
		}
	}
}

void AATBGameModeBase::NotifyAllPlayers(FString NotifyMsg)
{
	for (auto Player : AllPlayers)
	{
		if (IsValid(Player))
		{
			Player->ClientRPCNotifyMessage(NotifyMsg);
		}
	}
}

void AATBGameModeBase::NotifyRemainTime(float RemainTime)
{
	for (auto Player : AllPlayers)
	{
		if (IsValid(Player))
		{
			Player->ClientRPCNotifyRemainTime(RemainTime);
		}
	}
}

void AATBGameModeBase::SpawnBots()
{
	if (IsValid(AIBotClass))
	{
		for (int32 i = 0; i < BotSpawnCount; ++i)
		{
			FVector Pos = RandomPos();
			if (Pos != FVector::ZeroVector)
			{
				GetWorld()->SpawnActor<AATBBotCharacter>(AIBotClass, Pos, FRotator::ZeroRotator);
			}
		}
	}

}

void AATBGameModeBase::GamePlayerReady()
{
	if (AllPlayers.Num() != GameState->PlayerArray.Num())
	{
		SettingPlayers();
	}

	if (MaxPlayers != AllPlayers.Num())
	{
		NotifyAllPlayers(TEXT("Waiting For All Players..."));
	}
	else
	{
		AATBGameStateBase* ATBGS = GetGameState<AATBGameStateBase>();
		if (IsValid(ATBGS))
		{
			ATBGS->CurMatchType = EMatchType::Ready;
		}
	}
}

void AATBGameModeBase::PlayerDead(AATBPlayerController* Player)
{
	if (ALobbyPlayerState* LobbyPS = Player->GetPlayerState<ALobbyPlayerState>())
	{
		if (LobbyPS->PlayerInfo.PlayerJob == EPlayerJob::Theif)
		{
			LobbyPS->PlayerInfo.bIsArrest = true;
			ThiefPlayers.Remove(Player);
			DeadPlayers.Add(Player);
		}
		else if (LobbyPS->PlayerInfo.PlayerJob == EPlayerJob::Police)
		{
			LobbyPS->PlayerInfo.bIsArrest = true;
			PolicePlayers.Remove(Player);
			DeadPlayers.Add(Player);
		}
		UpdateThievesCount();
	}
}

void AATBGameModeBase::ReturnLobby()
{
	FString MapName = TEXT("/Game/ATB/Maps/Lobby");
	FString URL = MapName + TEXT("?listen");
	GetWorld()->ServerTravel(URL, true);
}

void AATBGameModeBase::JudgeGame()
{
	if (PolicePlayers.Num() <= 0)
	{
		// 도둑 승리
		bIsPoliceWin = false;
	}
	else if (ThiefPlayers.Num() <= 0)
	{
		// 경찰 승리
		bIsPoliceWin = true;
	}
	else if (PlayTime <= 0)
	{
		// 도둑 승리
		bIsPoliceWin = false;
	}
	

	AATBGameStateBase* ATBGS = GetGameState<AATBGameStateBase>();

	if (IsValid(ATBGS))
	{
		ATBGS->CurMatchType = EMatchType::None;
	}
}

void AATBGameModeBase::PlayerAllStop()
{
	TArray<FPlayerInfo> Infos;

	for (AATBPlayerController* ATBPC : AllPlayers)
	{
		if (ALobbyPlayerState* LobbyPS = ATBPC->GetPlayerState<ALobbyPlayerState>())
		{
			Infos.Add(LobbyPS->PlayerInfo);
		}
	}

	for (AATBPlayerController* ATBPC : AllPlayers) 
	{
		ATBPC->ClientRPCFinishGameProcessing(Infos, bIsPoliceWin);
	}
}

void AATBGameModeBase::UpdateThievesCount()
{
	if (AATBGameStateBase* ATBGS = GetGameState<AATBGameStateBase>())
	{
		ATBGS->ThievesCount = ThiefPlayers.Num();
		if (ThiefPlayers.Num() == 0 || PolicePlayers.Num() == 0)
		{
			ATBGS->CurMatchType = EMatchType::Ending;
		}
	}
}

FVector AATBGameModeBase::RandomPos()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!IsValid(NavSystem))
	{
		return FVector::ZeroVector;
	}
	
	FNavLocation RandomLocation;
	if (NavSystem->GetRandomPoint(RandomLocation))
	{
		RandomLocation.Location.Z += 50.0f;
		return RandomLocation.Location;
	}

	return FVector::ZeroVector;
}

void AATBGameModeBase::PoliceBuffAdded()
{
	for (const auto& Police : PolicePlayers)
	{
		if (AATBPlayerCharacter* Player = Cast<AATBPlayerCharacter>(Police->GetPawn()))
		{
			if (UStatusComponent* StatusComp = Player->FindComponentByClass<UStatusComponent>())
			{
				if (StatusComp->bIsBuffApply == false)
				{
					StatusComp->BuffApply();
				}
			}
		}
	}
}

