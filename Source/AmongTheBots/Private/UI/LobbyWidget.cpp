// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyWidget.h"
#include "Components/Button.h"
#include "UI/LobbyPlayerBox.h"
#include "Components/ScrollBox.h"
#include "Controller/LobbyPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"


void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedStartGame);
	ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedExitGame);
}

void ULobbyWidget::AddPlayerList(FString Playername)
{
	if (IsValid(LobbyPlayerBoxClass))
	{
		ULobbyPlayerBox* PlayerBox = CreateWidget<ULobbyPlayerBox>(this, LobbyPlayerBoxClass);
		if (IsValid(PlayerBox))
		{
			PlayerBox->SetPlayerName(Playername);
			PlayerListBox->AddChild(PlayerBox);
		}
	}
}

void ULobbyWidget::RemoveListBoxAll()
{
	PlayerListBox->ClearChildren(); // 로비 동기화를 위해 로비에 플레이어가 들어오면 전부 제거후 다시 생성
}

void ULobbyWidget::OnClickedStartGame()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC))
		{
			LobbyPC->ServerRPCStartGame();
		}
	}
}

void ULobbyWidget::OnClickedExitGame()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC))
		{
			LobbyPC->ServerRPCExitGame();
		}
	}
}

void ULobbyWidget::ShowStartGameButton(bool bIsShow)
{
	StartButton->SetVisibility(bIsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}




