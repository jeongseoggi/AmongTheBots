// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_Attack.h"
#include "Weapon/ATBPoliceWeapon.h"
#include "Weapon/WeaponActor.h"

void UAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(MeshComp))
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();

	if (!IsValid(Owner))
	{
		return;
	}

	UATBPoliceWeapon* PoliceWeapon = Owner->FindComponentByClass<UATBPoliceWeapon>();

	if (!IsValid(PoliceWeapon))
	{
		return;
	}

	if (AWeaponActor* WeaponActor = PoliceWeapon->CurrentWeapon)
	{
		WeaponActor->Attack();
	}

}
