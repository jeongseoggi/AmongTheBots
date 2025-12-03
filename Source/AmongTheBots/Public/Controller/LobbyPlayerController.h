// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class ULobbyWidget;
/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

	void AddLobbyPlayerList(FString PlayerName);

	void RemoveLobbyPlayerList();

	UFUNCTION(Server, Reliable)
	void ServerRPCStartGame();

	UFUNCTION(Server,Reliable)
	void ServerRPCExitGame();

	UFUNCTION(Client, Reliable)
	void ClientRPCExitGame();

	UFUNCTION()
	void SuccessTravel();
	
	UFUNCTION()
	void LobbyButtonSetting(int32 PlayerIndex);

	void PostSeamlessTravel() override;

	void InitUI();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<ULobbyWidget> LobbyWidgetInstance;

	UFUNCTION(Server,Reliable)
	void ServerRPCJoinLobby();

};
