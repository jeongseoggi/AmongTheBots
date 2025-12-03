// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ATBGameStateBase.generated.h"

UENUM(BlueprintType)
enum class EMatchType : uint8
{
	None,
	Ready,
	Playing,
	Ending
};

/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API AATBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_MatchType)
	EMatchType CurMatchType = EMatchType::Ready;

	UFUNCTION()
	void NotifyPlayers(FString NotifyMsg);

	UPROPERTY(ReplicatedUsing = OnRep_ThievesCount)
	int32 ThievesCount;

protected:
	UFUNCTION()
	void OnRep_MatchType();

	UFUNCTION()
	void OnRep_ThievesCount();

	void DoorSetting();
};
