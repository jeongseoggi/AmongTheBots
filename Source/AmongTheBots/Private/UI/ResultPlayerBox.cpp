// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResultPlayerBox.h"
#include "Components/TextBlock.h"

void UResultPlayerBox::SetResultText(TArray<FString> ResultArr)
{
	PlayerText->SetText(FText::FromString(ResultArr[0]));
	JobText->SetText(FText::FromString(ResultArr[1]));
	ArrestText->SetText(FText::FromString(ResultArr[2]));
}
