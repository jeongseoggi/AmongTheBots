// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyPlayerBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void ULobbyPlayerBox::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULobbyPlayerBox::SetPlayerName(FString PlayerName)
{
	PlayerNameText->SetText(FText::FromString(PlayerName));
}
