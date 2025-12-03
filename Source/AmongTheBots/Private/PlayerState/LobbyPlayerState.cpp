// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Controller/LobbyPlayerController.h"

ALobbyPlayerState::ALobbyPlayerState()
{

}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerInfo);
}

void ALobbyPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(PlayerState);
	if (IsValid(LobbyPS))
	{
		LobbyPS->PlayerInfo = PlayerInfo;
	}
}

void ALobbyPlayerState::ClientSettingForPlayerIndex_Implementation()
{
	if (HasAuthority() == false)
	{
		OnPlayerIndexChanged.Broadcast(PlayerInfo.PlayerIndex);
	}
}

void ALobbyPlayerState::OnRep_PlayerInfo()
{
	OnPlayerIndexChanged.Broadcast(PlayerInfo.PlayerIndex);
}


