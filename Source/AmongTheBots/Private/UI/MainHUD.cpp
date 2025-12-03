// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/ATBPlayerCharacter.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	AATBPlayerCharacter* Player = Cast<AATBPlayerCharacter>(GetOwningPlayerPawn());
	if (IsValid(Player))
	{
		Player->InitHUD(this);
	}

}

void UMainHUD::SetSystemMessageText(FString Message)
{
	SystemMessageText->SetText(FText::FromString(Message));
}

void UMainHUD::SetRemainThiefText(int32 Remain)
{
	FString BasicStr = TEXT("Alive Thieves : ");
	FString CombinedStr = BasicStr + FString::FromInt(Remain);
	RemainThiefText->SetText(FText::FromString(CombinedStr));
}

void UMainHUD::SetRemainTimeText(float Time)
{
	int32 TotalSeconds = FMath::RoundToInt(Time);

	int32 Min = TotalSeconds / 60;
	int32 Sec = TotalSeconds % 60;

	FString FomatString = FString::Printf(TEXT("%02d:%02d"), Min, Sec);
	TimerText->SetText(FText::FromString(FomatString));
}

void UMainHUD::InitHPBar(float MaxHP)
{
	HPBar->SetPercent(MaxHP * 0.01f);
}

void UMainHUD::SetHPBar(float CurrentHP)
{
	HPBar->SetPercent(CurrentHP * 0.01f);
}
