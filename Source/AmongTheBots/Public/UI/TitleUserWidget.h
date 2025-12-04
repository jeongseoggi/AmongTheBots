// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleUserWidget.generated.h"


class UButton;
class UEditableText;


/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API UTitleUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UButton> JoinButton;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<UEditableText> NickNameTextable;

	UFUNCTION()
	void OnClickedJoinGame();

	UFUNCTION()
	void OnClickedExitGame();

};
