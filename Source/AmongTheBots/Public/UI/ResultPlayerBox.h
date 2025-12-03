// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultPlayerBox.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API UResultPlayerBox : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetResultText(TArray<FString> ResultArr);

protected:
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UTextBlock> PlayerText;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UTextBlock> JobText;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UTextBlock> ArrestText;

};
