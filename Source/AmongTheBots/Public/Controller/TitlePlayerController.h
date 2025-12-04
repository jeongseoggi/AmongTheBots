// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePlayerController.generated.h"

class UTitleUserWidget;
/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ExitGame();

	UFUNCTION(BlueprintCallable)
	void JoinGame();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTitleUserWidget> TitleWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UTitleUserWidget> TitleWidgetInstacne;
};
