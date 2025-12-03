// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ATBAIController.generated.h"

/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API AATBAIController : public AAIController
{
	GENERATED_BODY()

public:
	AATBAIController();

	UFUNCTION(BlueprintPure)
	UBlackboardComponent* GetBlackBoardComp() const;
	void StartBehaviorTree();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackBoardComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

};
