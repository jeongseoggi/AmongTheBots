// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameModeBase.generated.h"

class ALobbyPlayerController;
/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API ALobbyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void JoinPlayerInLobby(ALobbyPlayerController* LobbyPC);

	void FirstSyncLobbyList();

	void StartGameServerTravel();

	void ExitGame(int32 Index);

	virtual void PostSeamlessTravel() override;

	void SettingPlayers();

	void PostSeamlessTravelPlayerInfos();

protected:
	TArray<TObjectPtr<ALobbyPlayerController>> LobbyPlayerControllers;

private:
	void ReIndexingPlayerInfos();
	int32 RandomJob();
	void PreGameStartJobSetting(); // 게임 시작 전 직업 배정
};
