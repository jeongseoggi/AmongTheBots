// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Controller/TitlePlayerController.h"
#include "Game/ATBGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UTitleUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	JoinButton->OnClicked.AddDynamic(this, &UTitleUserWidget::OnClickedJoinGame);
	ExitButton->OnClicked.AddDynamic(this, &UTitleUserWidget::OnClickedExitGame);

}

void UTitleUserWidget::OnClickedJoinGame()
{
	UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
	if (IsValid(GI))
	{
		if (UATBGameInstance* ATBGI = Cast<UATBGameInstance>(GI))
		{
			FString NickName = NickNameTextable->GetText().ToString();
			ATBGI->SetPlayerNickName(NickName);
		}
	}


	APlayerController* PC = GetOwningPlayer();
	if (IsValid(PC))
	{
		if (ATitlePlayerController* TitlePC = Cast<ATitlePlayerController>(PC))
		{
			TitlePC->JoinGame();
		}
	}
}

void UTitleUserWidget::OnClickedExitGame()
{
	APlayerController* PC = GetOwningPlayer();
	if (IsValid(PC))
	{
		if (ATitlePlayerController* TitlePC = Cast<ATitlePlayerController>(PC))
		{
			TitlePC->ExitGame();
		}
	}
}
