// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LobbyGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/LobbyPlayerController.h"
#include "PlayerState/LobbyPlayerState.h"

void ALobbyGameStateBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerInfos);
}

void ALobbyGameStateBase::OnRep_PlayerInfos()
{
	if (!HasAuthority())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC))
		{
			if (ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC))
			{
				LobbyPC->RemoveLobbyPlayerList();
				for (const FPlayerInfo& Infos : PlayerInfos)
				{
					LobbyPC->AddLobbyPlayerList(Infos.PlayerName);
				}
			}
		}
	}
}

void ALobbyGameStateBase::RemovePlayerInfos(int32 Index)
{
	for (int32 i = 0; i < PlayerInfos.Num(); ++i)
	{
		if (PlayerInfos[i].PlayerIndex == Index)
		{
			PlayerInfos.RemoveAt(i);
			break;
		}
	}
}

void ALobbyGameStateBase::MulticastRPCSuccessTravel_Implementation()
{
	if (!HasAuthority())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC))
		{
			if (ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC))
			{
				LobbyPC->SuccessTravel();
			}
		}
	}
}
