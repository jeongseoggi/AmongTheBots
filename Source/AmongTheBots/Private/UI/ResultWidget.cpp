// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResultWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Controller/ATBPlayerController.h"
#include "UI/ResultPlayerBox.h"
#include "PlayerState/LobbyPlayerState.h"
#include "Controller/ATBPlayerController.h"

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackLobbyButton->OnClicked.AddDynamic(this, &ThisClass::ReturnLobby);
}



void UResultWidget::SettingResult(const TArray<FPlayerInfo>& PlayerInfos)
{
	for (FPlayerInfo Info : PlayerInfos)
	{
		if (IsValid(ResultPlayerBox))
		{
			UResultPlayerBox* ResultPlayerBoxInstance = CreateWidget<UResultPlayerBox>(this, ResultPlayerBox);

			if (IsValid(ResultPlayerBoxInstance))
			{
				PlayerVerticalBox->AddChild(ResultPlayerBoxInstance);

				TArray<FString> ResultStringArr;
				ResultStringArr.Add(Info.PlayerName);
				FString EnumToString = StaticEnum<EPlayerJob>()->GetNameStringByValue((int64)Info.PlayerJob);
				ResultStringArr.Add(EnumToString);
				ResultStringArr.Add(Info.bIsArrest ? TEXT("Arrest") : TEXT("No"));
				ResultPlayerBoxInstance->SetResultText(ResultStringArr);
			}
		}
	}
}

void UResultWidget::SettingWinnerText(bool bIsPoliceWin)
{
	APlayerController* PC = GetOwningPlayer();

	if (IsValid(PC))
	{
		if (AATBPlayerController* ATBPC = Cast<AATBPlayerController>(PC))
		{
			if (ALobbyPlayerState* LobbyPC = ATBPC->GetPlayerState<ALobbyPlayerState>())
			{
				if (LobbyPC->PlayerInfo.PlayerJob == EPlayerJob::Police && bIsPoliceWin || LobbyPC->PlayerInfo.PlayerJob == EPlayerJob::Theif && !bIsPoliceWin)
				{
					WinnerText->SetText(FText::FromString(TEXT("You Win!")));
				}
				else
				{
					WinnerText->SetText(FText::FromString(TEXT("You Lose!")));
				}
			}
		}
	}
}

void UResultWidget::ReturnLobby()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AATBPlayerController* ATBPC = Cast<AATBPlayerController>(PC))
		{
			ATBPC->ServerRPCReturnLobby();
		}
	}
}
