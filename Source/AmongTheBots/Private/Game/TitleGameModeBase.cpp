// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TitleGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ATitleGameModeBase::ATitleGameModeBase()
{

}

void ATitleGameModeBase::JoinServer()
{
	FString IpAddress = "127.0.0.1:17777";
	UGameplayStatics::OpenLevel(GetWorld(), FName(*IpAddress), true);
}
