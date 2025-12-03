// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UButton;
class UScrollBox;
/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void AddPlayerList(FString Playername);

	UFUNCTION()
	void RemoveListBoxAll();

	UFUNCTION()
	void OnClickedStartGame();

	UFUNCTION()
	void OnClickedExitGame();

	void ShowStartGameButton(bool bIsShow);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UScrollBox> PlayerListBox;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ULobbyPlayerBox> LobbyPlayerBoxClass;
};
