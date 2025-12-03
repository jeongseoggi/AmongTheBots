#include "Gimmick/Door.h"
#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneComp);

}

void ADoor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bIsVisible);
}

void ADoor::OnRep_DoorVisible()
{
	SetActorHiddenInGame(!bIsVisible);
	SetActorEnableCollision(bIsVisible);
}

void ADoor::SetDoorState(bool bIsShow)
{
	if (HasAuthority())
	{
		bIsVisible = bIsShow;
		OnRep_DoorVisible();


		if (bIsShow == false)
		{
			FTimerHandle Timer;
			GetWorldTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([this]()
				{
					SetDoorState(true);
				}),
				10.0f,
				false);
		}
	}
}
