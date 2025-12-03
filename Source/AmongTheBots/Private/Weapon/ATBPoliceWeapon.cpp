// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ATBPoliceWeapon.h"
#include "Character/ATBPlayerCharacter.h"
#include "Weapon/WeaponActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UATBPoliceWeapon::UATBPoliceWeapon()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UATBPoliceWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void UATBPoliceWeapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, CurrentWeapon);
}

void UATBPoliceWeapon::OnRep_CurrentWeapon()
{
	AATBPlayerCharacter* OwnerCharacter = Cast<AATBPlayerCharacter>(GetOwner());

	if (OwnerCharacter && CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(
			OwnerCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("Weapon_R")        // 너가 사용 중인 소켓 이름
		);
	}
}


void UATBPoliceWeapon::EquipWeapon(TSubclassOf<AWeaponActor> WeaponClass)
{
	if (GetOwner()->HasAuthority() == false)
	{
		return;
	}

	AATBPlayerCharacter* OwnerCharacter = Cast<AATBPlayerCharacter>(GetOwner());

	if (IsValid(OwnerCharacter))
	{
		FActorSpawnParameters Params;

		Params.Owner = OwnerCharacter;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClass, Params);

		if (CurrentWeapon)
		{
			CurrentWeapon->AttachToComponent(OwnerCharacter->GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				TEXT("Weapon_R")  // 손 소켓 이름
			);
		}
	}
}



