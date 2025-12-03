// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API UBTTaskNode_FindRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_FindRandomLocation();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* nodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector LocationKey;

	UPROPERTY(EditAnywhere)
	float SearchRadius = 1000.0f;
};
