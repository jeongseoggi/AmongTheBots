// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerIndexChanged, int32, NewPlayerIndex);

UENUM(BlueprintType)
enum class EPlayerJob : uint8
{
	None,
	Police,
	Theif
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	EPlayerJob PlayerJob;

	UPROPERTY()
	int32 PlayerIndex;

	UPROPERTY()
	bool bIsArrest;
};


/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ALobbyPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(APlayerState* PlayerState) override;

	UFUNCTION()
	FORCEINLINE void SetLobbyPlayerIndex(int32 PlayerIndex) { PlayerInfo.PlayerIndex = PlayerIndex; }

	UPROPERTY()
	FOnPlayerIndexChanged OnPlayerIndexChanged;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PlayerInfo)
	FPlayerInfo PlayerInfo; 

	UFUNCTION(Client, Reliable)
	void ClientSettingForPlayerIndex();

	UPROPERTY(Replicated)
	int32 MaxPlayer;

protected:
	UFUNCTION()
	void OnRep_PlayerInfo();
};
