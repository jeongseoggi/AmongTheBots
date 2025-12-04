// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ATBGameModeBase.generated.h"

class AATBPlayerController;
class ALobbyPlayerState;
class AATBBotCharacter;
/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API AATBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AATBGameModeBase();
	virtual void BeginPlay() override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void PostSeamlessTravel() override;

	void OnMainTimerElapsed();

	void SettingPlayers();

	UFUNCTION()
	void PlayerDead(AATBPlayerController* Player);

	void ReturnLobby();

protected:
	TArray<TObjectPtr<AATBPlayerController>> PolicePlayers;
	TArray<TObjectPtr<AATBPlayerController>> ThiefPlayers;
	TArray<TObjectPtr<AATBPlayerController>> DeadPlayers;
	TArray<TObjectPtr<AATBPlayerController>> AllPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TSubclassOf<AATBBotCharacter> AIBotClass;

	UFUNCTION()
	void NotifyAllPlayers(FString NotifyMsg);

	UFUNCTION()
	void NotifyRemainTime(float RemainTime);

	UFUNCTION()
	void SpawnBots();

	UFUNCTION()
	void GamePlayerReady();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReadyTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayTime;

private:
	FTimerHandle ServerTimer;

	void JudgeGame();

	void PlayerAllStop();

	void UpdateThievesCount();

	FVector RandomPos();

	void PoliceBuffAdded();

	bool bIsPoliceWin;

	int32 BotSpawnCount = 10;

	int32 MaxPlayers;
};
