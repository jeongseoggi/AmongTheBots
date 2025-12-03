// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_FindRandomLocation.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/ATBAIController.h"
#include "NavigationSystem.h"

UBTTaskNode_FindRandomLocation::UBTTaskNode_FindRandomLocation()
{
	NodeName = TEXT("Find Random Location");

	LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_FindRandomLocation, LocationKey));
}

EBTNodeResult::Type UBTTaskNode_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* nodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!IsValid(AIController))
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();

	if (!IsValid(Pawn))
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!IsValid(NavSystem))
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation NavLocation;
	bool bFound = NavSystem->GetRandomReachablePointInRadius(
		Pawn->GetActorLocation(),
		SearchRadius,
		NavLocation
	);

	if (bFound)
	{
		UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
		if (IsValid(BBComp))
		{
			BBComp->SetValueAsVector(LocationKey.SelectedKeyName, NavLocation.Location);

			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Type();
}
