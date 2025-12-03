// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"


class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct();

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UTextBlock> TimerText;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UTextBlock> SystemMessageText;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UTextBlock> RemainThiefText;

	void SetSystemMessageText(FString Message);

	void SetRemainThiefText(int32 Remain);

	void SetRemainTimeText(float Time);

	void InitHPBar(float MaxHP);

	void SetHPBar(float CurrentHP);
};
