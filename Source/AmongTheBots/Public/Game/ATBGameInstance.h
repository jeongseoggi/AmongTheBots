// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ATBGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API UATBGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FORCEINLINE const FString& GetPlayerNickName() const { return PlayerNickName; }
	void SetPlayerNickName(const FString& NickName);
private:
	FString PlayerNickName;
	
};
