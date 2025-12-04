// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/LobbyPlayerController.h"
#include "UI/LobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LobbyGameModeBase.h"
#include "PlayerState/LobbyPlayerState.h"
#include "Game/ATBGameInstance.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitUI();
}

void ALobbyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();	
	if (ALobbyPlayerState* LobbyPS = GetPlayerState<ALobbyPlayerState>())
	{
		LobbyPS->OnPlayerIndexChanged.AddDynamic(this, &ALobbyPlayerController::LobbyButtonSetting);
	}

	UGameInstance* GI = UGameplayStatics::GetGameInstance(this);
	if (IsValid(GI))
	{
		if (UATBGameInstance* ATBGI = Cast<UATBGameInstance>(GI))
		{
			const FString& NickName = ATBGI->GetPlayerNickName();
			ServerRPCJoinLobby(NickName); //내가 로비에 접속 했음을 알림 (다른 플레이어에게)	
		}
	}
}

void ALobbyPlayerController::AddLobbyPlayerList(FString PlayerName)
{
	LobbyWidgetInstance->AddPlayerList(PlayerName);
}

void ALobbyPlayerController::RemoveLobbyPlayerList()
{
	if (IsValid(LobbyWidgetInstance))
	{
		LobbyWidgetInstance->RemoveListBoxAll();
	}
}

void ALobbyPlayerController::ServerRPCStartGame_Implementation()
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM))
	{
		if (ALobbyGameModeBase* LobbyGM = Cast<ALobbyGameModeBase>(GM))
		{
			LobbyGM->StartGameServerTravel();
		}
	}
}

void ALobbyPlayerController::ServerRPCJoinLobby_Implementation(const FString& NickName)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);

	if (IsValid(GM))
	{
		if (ALobbyGameModeBase* LobbyGM = Cast<ALobbyGameModeBase>(GM))
		{
			LobbyGM->JoinPlayerInLobby(this, NickName);
			LobbyGM->FirstSyncLobbyList();
		}
	}

}

void ALobbyPlayerController::ClientRPCExitGame_Implementation()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}

void ALobbyPlayerController::ServerRPCExitGame_Implementation()
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM))
	{
		if (ALobbyGameModeBase* LobbyGM = Cast<ALobbyGameModeBase>(GM))
		{
			if (ALobbyPlayerState* LobbyPS = GetPlayerState<ALobbyPlayerState>())
			{
				LobbyGM->ExitGame(LobbyPS->PlayerInfo.PlayerIndex);
			}
		}
	}
}

void ALobbyPlayerController::SuccessTravel()
{
	LobbyWidgetInstance->RemoveFromViewport();
}

void ALobbyPlayerController::LobbyButtonSetting(int32 NewPlayerIndex)
{
	LobbyWidgetInstance->ShowStartGameButton(NewPlayerIndex == 1 ? true : false);
}

void ALobbyPlayerController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	InitUI();
}

void ALobbyPlayerController::InitUI()
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	if (IsValid(LobbyWidgetClass) && LobbyWidgetInstance == nullptr)
	{
		LobbyWidgetInstance = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
		if (IsValid(LobbyWidgetInstance))
		{
			LobbyWidgetInstance->AddToViewport();

			FInputModeUIOnly UIMode;
			SetInputMode(UIMode);
			bShowMouseCursor = true;
		}
	}
}
