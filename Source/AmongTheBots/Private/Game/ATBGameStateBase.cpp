// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ATBGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/LobbyPlayerState.h"
#include "Controller/ATBPlayerController.h"
#include "Game/ATBGameModeBase.h"

void AATBGameStateBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, CurMatchType);
	DOREPLIFETIME(ThisClass, ThievesCount);
}

void AATBGameStateBase::OnRep_MatchType()
{
	switch (CurMatchType)
	{
	case EMatchType::None:
		break;
	case EMatchType::Ready:
		break;
	case EMatchType::Playing:
		{
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				if (AATBPlayerController* ATBPC = Cast<AATBPlayerController>(PC))
				{
					if (ALobbyPlayerState* LobbyPS = ATBPC->GetPlayerState<ALobbyPlayerState>())
					{
						FString NotifyMsg;
						if (LobbyPS->PlayerInfo.PlayerJob == EPlayerJob::Police)
						{
							NotifyMsg = TEXT("Find the thieves who are hiding in time!");
						}
						else
						{
							NotifyMsg = TEXT("Survive away from the police in time!");
						}

						NotifyPlayers(NotifyMsg);
					}
				}
			}
			DoorSetting();
		}
		break;
	case EMatchType::Ending:
		break;
	default:
		break;
	}
}

void AATBGameStateBase::OnRep_ThievesCount()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (AATBPlayerController* ATBPC = Cast<AATBPlayerController>(PC))
		{
			ATBPC->ClientRPCNotifyRemainThieves(ThievesCount);
		}
	}
}

void AATBGameStateBase::DoorSetting()
{

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(PC))
	{
		AATBPlayerController* ATBPC = Cast<AATBPlayerController>(PC);
		ATBPC->DoorSetting();
	}
}


void AATBGameStateBase::NotifyPlayers(FString NotifyMsg)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (AATBPlayerController* ATBPC = Cast<AATBPlayerController>(PC))
		{
			ATBPC->ClientRPCNotifyMessage(NotifyMsg);
		}
	}
}

