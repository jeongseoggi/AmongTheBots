#include "Status/StatusComponent.h"
#include "Net/UnrealNetwork.h"
#include "Character/ATBPlayerCharacter.h"

UStatusComponent::UStatusComponent() :
	MaxHP(100.0f)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	CurrentHP = MaxHP;
}

void UStatusComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentHP);
}

float UStatusComponent::ApplyDamage(float InDamage)
{
	if (IsValid(GetOwner()) == false || GetOwner()->HasAuthority() == false)
	{
		return 0.0f;
	}

	const float PrevHP = CurrentHP;
	const float ActualDamage = FMath::Clamp(InDamage, 0, PrevHP);

	SetCurrentHP(PrevHP - ActualDamage);

	return ActualDamage;
}


void UStatusComponent::SetCurrentHP(float InCurrentHP)
{
	if (IsValid(GetOwner()) == false || GetOwner()->HasAuthority() == false)
	{
		return;
	}
	CurrentHP = InCurrentHP;
	OnCurrentHPChanged.Broadcast(CurrentHP);
}

void UStatusComponent::SetMaxHP(float InMaxHP)
{
	if (IsValid(GetOwner()) == false || GetOwner()->HasAuthority() == false)
	{
		return;
	}

	MaxHP = InMaxHP;

	if (MaxHP < KINDA_SMALL_NUMBER)
	{
		MaxHP = 0.1f;
	}
}

void UStatusComponent::OnRep_CurrentHP()
{
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		AATBPlayerCharacter* OwnerCharacter = Cast<AATBPlayerCharacter>(GetOwner());

		if (IsValid(OwnerCharacter))
		{
			OwnerCharacter->DeadProcessing();
		}
	}

	OnCurrentHPChanged.Broadcast(CurrentHP);
}

void UStatusComponent::BuffApply()
{
	
	if (AActor* Player = GetOwner())
	{
		if (AATBPlayerCharacter* PlayerCharacter = Cast<AATBPlayerCharacter>(Player))
		{
			if (PlayerCharacter->HasAuthority() && bIsBuffApply == false)
			{
				PlayerCharacter->SetMoveSpeed();
				bIsBuffApply = true;
			}
		}
	}
}
