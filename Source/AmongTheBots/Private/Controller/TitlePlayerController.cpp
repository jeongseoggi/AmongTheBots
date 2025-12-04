// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/TitlePlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/TitleGameModeBase.h"
#include "UI/TitleUserWidget.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly UIOnly;
	SetInputMode(UIOnly);

	bShowMouseCursor = true;

	if (IsLocalController())
	{
		if (IsValid(TitleWidgetClass))
		{
			TitleWidgetInstacne = CreateWidget<UTitleUserWidget>(this, TitleWidgetClass);
			if (IsValid(TitleWidgetInstacne))
			{
				TitleWidgetInstacne->AddToViewport();
			}
		}
	}
}

void ATitlePlayerController::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}

void ATitlePlayerController::JoinGame()
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);

	if (ATitleGameModeBase* TitleGM = Cast<ATitleGameModeBase>(GM))
	{
		TitleGM->JoinServer();
	}
}
