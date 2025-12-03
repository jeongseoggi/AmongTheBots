// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerBox.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API ULobbyPlayerBox : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;
	void SetPlayerName(FString PlayerName);

	UPROPERTY(meta=(bindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;
	
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UImage> PlayerBoxBG;

};
