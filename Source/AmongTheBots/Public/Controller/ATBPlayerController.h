// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ATBPlayerController.generated.h"


class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UMainHUD;
class UResultWidget;
struct FPlayerInfo;

UCLASS()
class AMONGTHEBOTS_API AATBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AATBPlayerController();

	virtual void BeginPlay() override;

	void DoorSetting();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> AttackInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> MappingContext;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UMainHUD> MainHUDClass;

	UPROPERTY()
	TObjectPtr<UMainHUD> MainHUDInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UResultWidget> ResultWidgetClass;

	UPROPERTY()
	TObjectPtr<UResultWidget> ResultWidgetnstance;




	// 서버에게 준비가 완료되어 HUD에 표기할 정보들을 요청하는 함수
	UFUNCTION(Server, Reliable)
	void ServerRPCPlayReady();

	UFUNCTION(Client, Reliable)
	void ClientRPCNotifyMessage(const FString& Message);


	UFUNCTION(Client, Reliable)
	void ClientRPCNotifyRemainTime(float Time);


	UFUNCTION(Client, Reliable)
	void ClientRPCNotifyRemainThieves(int32 Count);

	UFUNCTION(Client, Reliable)
	void ClientRPCFinishGameProcessing(const TArray<FPlayerInfo>& PlayerInfos, bool bIsPoliceWin);

	UFUNCTION(Server,Reliable)
	void ServerRPCDoorSetting();

	UFUNCTION(Server,Reliable)
	void ServerRPCReturnLobby();
};
