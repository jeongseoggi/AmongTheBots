// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ATBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AATBAIController::AATBAIController()
{
	BlackBoardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void AATBAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AATBAIController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BlackBoardComp))
	{
		StartBehaviorTree();
	}

}

UBlackboardComponent* AATBAIController::GetBlackBoardComp() const
{
	return BlackBoardComp;
}

void AATBAIController::StartBehaviorTree()
{
	if (IsValid(BehaviorTreeAsset))
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Behavior Tree Asset not set!"));
	}
}
