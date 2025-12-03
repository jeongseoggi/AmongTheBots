// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Character/ATBPlayerCharacter.h"
#include "Character/ATBBotCharacter.h"
#include "Engine/DamageEvents.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collison"));
	SphereComponent->SetupAttachment(WeaponMesh);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::OnSphereBeginOverlap);
}


// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
 		OverlappedEnemies.AddUnique(OtherActor);
	}
}

void AWeaponActor::Attack()
{
	SphereComponent->SetGenerateOverlapEvents(true);
}

void AWeaponActor::AttackEnd()
{
	SphereComponent->SetGenerateOverlapEvents(false);
	DamageActors();
	OverlappedEnemies.Empty();
}


void AWeaponActor::DamageActors()
{
	if (GetOwner()->HasAuthority() == false)
	{
		if (AATBPlayerCharacter* MyCharacter = Cast<AATBPlayerCharacter>(GetOwner()))
		{
			MyCharacter->SetOverlappedEnemies(OverlappedEnemies);
		}
	}
}



