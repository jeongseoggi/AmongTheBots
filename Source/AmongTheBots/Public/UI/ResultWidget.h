// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

class UButton;
class UTextBlock;
class UResultPlayerBox;
class UVerticalBox;
class AATBPlayerController;
struct FPlayerInfo;
/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

	void SettingResult(const TArray<FPlayerInfo>& PlayerInfos);
	void SettingWinnerText(bool bIsPoliceWin);

	UFUNCTION()
	void ReturnLobby();

protected:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<UButton> BackLobbyButton;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UTextBlock> WinnerText;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UVerticalBox> PlayerVerticalBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UResultPlayerBox> ResultPlayerBox;

};
