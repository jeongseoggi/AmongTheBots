// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ATBPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Game/ATBGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MainHUD.h"
#include "UI/ResultWidget.h"
#include "PlayerState/LobbyPlayerState.h"
#include "Gimmick/Door.h"
#include "EngineUtils.h"

AATBPlayerController::AATBPlayerController()
{

}

void AATBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController())
	{
		if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				SubSystem->AddMappingContext(MappingContext, 0);
			}
		}

		FInputModeGameOnly GameOnly;
		SetInputMode(GameOnly);

		if (IsValid(MainHUDClass))
		{
			MainHUDInstance = CreateWidget<UMainHUD>(this, MainHUDClass);
			if (IsValid(MainHUDInstance))
			{
				MainHUDInstance->AddToViewport();
			}
		}
	}
}

void AATBPlayerController::DoorSetting()
{
	ServerRPCDoorSetting();
}

void AATBPlayerController::ServerRPCDoorSetting_Implementation()
{
	for (TActorIterator<ADoor> It(GetWorld()); It; ++It)
	{
		ADoor* Door = *It;
		if (Door && Door->HasAuthority())
		{
			Door->SetDoorState(false); // 서버에서 변경
		}
	}
}

void AATBPlayerController::ClientRPCNotifyRemainThieves_Implementation(int32 Count)
{
	if (IsValid(MainHUDInstance))
	{
		MainHUDInstance->SetRemainThiefText(Count);
	}
}

void AATBPlayerController::ClientRPCFinishGameProcessing_Implementation(const TArray<FPlayerInfo>& PlayerInfos, bool bIsPoliceWin)
{
	FInputModeUIOnly UIOnlyMode;
	SetInputMode(UIOnlyMode);
	SetPause(true);
	bShowMouseCursor = true;

	if (IsValid(ResultWidgetClass))
	{
		ResultWidgetnstance = CreateWidget<UResultWidget>(this, ResultWidgetClass);
		if (IsValid(ResultWidgetnstance))
		{
			ResultWidgetnstance->AddToViewport();
			ResultWidgetnstance->SettingResult(PlayerInfos);
			ResultWidgetnstance->SettingWinnerText(bIsPoliceWin);
		}
	}

}

void AATBPlayerController::ClientRPCNotifyRemainTime_Implementation(float Time)
{
	if (MainHUDInstance)
	{
		MainHUDInstance->SetRemainTimeText(Time);
	}
}

void AATBPlayerController::ClientRPCNotifyMessage_Implementation(const FString& Message)
{
	if (MainHUDInstance)
	{
		MainHUDInstance->SetSystemMessageText(Message);
	}
}

void AATBPlayerController::ServerRPCPlayReady_Implementation()
{
	if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this))
	{
		if (AATBGameModeBase* ATBGM = Cast<AATBGameModeBase>(GM))
		{
			//ATBGM->ClientReadyOk();
		}
	}
}

void AATBPlayerController::ServerRPCReturnLobby_Implementation()
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM))
	{
		if (AATBGameModeBase* ATBGM = Cast<AATBGameModeBase>(GM))
		{
			ATBGM->ReturnLobby();
		}
	}
}
