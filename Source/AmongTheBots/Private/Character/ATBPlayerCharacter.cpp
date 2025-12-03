// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ATBPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Controller/ATBPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Weapon/ATBPoliceWeapon.h"
#include "PlayerState/LobbyPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "Engine/DamageEvents.h"
#include "Character/ATBBotCharacter.h"
#include "Weapon/WeaponActor.h"
#include "Status/StatusComponent.h"
#include "UI/MainHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ATBGameModeBase.h"

AATBPlayerCharacter::AATBPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	PoliceWeaponComponent = CreateDefaultSubobject<UATBPoliceWeapon>(TEXT("Weapon"));
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("Status"));

	bCanAttack = true;
}



void AATBPlayerCharacter::MeleeAttackEnd()
{
	if (IsLocallyControlled())
	{
		ServerRPCMeleeAttackEnd();
	}

}

void AATBPlayerCharacter::ApplyAttack(AATBPlayerCharacter* InDamagedCharacter)
{
	if (InDamagedCharacter && HasAuthority() == false)
	{
		ServerRPCPerformMeleeHit(InDamagedCharacter);
	}
}

void AATBPlayerCharacter::SetOverlappedEnemies(TArray<TObjectPtr<AActor>> OverlappedEnemies)
{
	this->OverlappedEnemiesArray = OverlappedEnemies;
	if (IsLocallyControlled())
	{
		for (TObjectPtr<AActor> TargetActor : OverlappedEnemies)
		{
			if (AATBPlayerCharacter* OtherPlayer = Cast<AATBPlayerCharacter>(TargetActor))
			{
				if (OtherPlayer == this)
				{
					continue;
				}
				ServerRPCPerformMeleeHit(OtherPlayer);
			}
			else if (AATBBotCharacter* NPCCharacter = Cast<AATBBotCharacter>(TargetActor))
			{
				ServerRPCPerformMeleeHit(this);
			}
		}
	}
}

void AATBPlayerCharacter::InitHUD(UMainHUD* HUDWidget)
{
	if (IsValid(HUDWidget))
	{
		HUDWidget->InitHPBar(StatusComponent->GetMaxHP());
		StatusComponent->OnCurrentHPChanged.AddUObject(HUDWidget, &UMainHUD::SetHPBar);
	}
}

void AATBPlayerCharacter::DeadProcessing()
{
	if (IsLocallyControlled())
	{
		ServerRPCDeathProcessing();
	}
}

void AATBPlayerCharacter::SetMoveSpeed()
{
	if (HasAuthority())
	{
		NetUpdateFrequency = 150.f;
		MinNetUpdateFrequency = 60.f;

		UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement();

		if (IsValid(CharacterMovementComp))
		{
			float CurrentWalkSpeed = CharacterMovementComp->MaxWalkSpeed;
			CharacterMovementComp->MaxWalkSpeed = CurrentWalkSpeed * 2.0f;
			CharacterMovementComp->NetworkMaxSmoothUpdateDistance = 200.0f;

			UE_LOG(LogTemp, Warning, TEXT("%f"), CharacterMovementComp->MaxWalkSpeed);
		}
	}
}

// Called when the game starts or when spawned
void AATBPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AATBPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		if (ALobbyPlayerState* PS = GetPlayerState<ALobbyPlayerState>())
		{
			if (PS->PlayerInfo.PlayerJob == EPlayerJob::Police)
			{
				PoliceWeaponComponent->EquipWeapon(WeaponClass);
			}
		}
	}

}


void AATBPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (IsValid(InputComp))
	{
		if (IsLocallyControlled())
		{
			if (AATBPlayerController* PC = Cast<AATBPlayerController>(GetController()))
			{
				InputComp->BindAction(PC->MoveInput, ETriggerEvent::Triggered, this, &AATBPlayerCharacter::MoveAction);
				InputComp->BindAction(PC->LookInput, ETriggerEvent::Triggered, this, &AATBPlayerCharacter::LookAction);
				InputComp->BindAction(PC->JumpInput, ETriggerEvent::Triggered, this, &AATBPlayerCharacter::JumpAction);
				InputComp->BindAction(PC->AttackInput, ETriggerEvent::Started, this, &AATBPlayerCharacter::AttackAction);
			}
		}
	}
}

float AATBPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ALobbyPlayerState* LobbyPS = GetPlayerState<ALobbyPlayerState>();
	if (IsValid(LobbyPS))
	{
		StatusComponent->ApplyDamage(ActualDamage);
	}
	return ActualDamage;
}

void AATBPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bCanAttack);
}


void AATBPlayerCharacter::MoveAction(const FInputActionValue& MoveValue)
{
	const FVector2D MoveVec = MoveValue.Get<FVector2D>();
	if (Controller)
	{
		AddMovementInput(GetActorForwardVector(), MoveVec.X);
		AddMovementInput(GetActorRightVector(), MoveVec.Y);
	}
}

void AATBPlayerCharacter::LookAction(const FInputActionValue& LookValue)
{
	const FVector2D LookVec = LookValue.Get<FVector2D>();
	AddControllerYawInput(LookVec.X);
	AddControllerPitchInput(LookVec.Y);
}

void AATBPlayerCharacter::JumpAction(const FInputActionValue& JumpValue)
{
	bool IsJump = JumpValue.Get<bool>();

	if (IsJump)
	{
		Jump();
	}
}

void AATBPlayerCharacter::AttackAction(const FInputActionValue& AttackValue)
{
	if (ALobbyPlayerState* LobbyPS = GetPlayerState<ALobbyPlayerState>())
	{
		if (LobbyPS->PlayerInfo.PlayerJob == EPlayerJob::Police)
		{
			if (bCanAttack && GetCharacterMovement()->IsFalling() == false)
			{
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if (IsValid(AnimInstance))
				{
					ServerRPCMeleeAttack();
					AnimInstance->Montage_Play(MeleeAttackMontage);
				}
			}
		}
	}
}

void AATBPlayerCharacter::SetCanAttack(bool bIsCanAttack)
{
	bCanAttack = bIsCanAttack;
}

void AATBPlayerCharacter::OnRep_CanAttack()
{
	if (bCanAttack == true)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
}


void AATBPlayerCharacter::ServerRPCMeleeAttack_Implementation()
{
	bCanAttack = false;
	OnRep_CanAttack();

	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (IsValid(PlayerController) && GetController() != PlayerController)
		{
			AATBPlayerCharacter* OtherPlayerCharacter = Cast<AATBPlayerCharacter>(PlayerController->GetPawn());
			if (IsValid(OtherPlayerCharacter))
			{
				OtherPlayerCharacter->ClientRPCPlayMeleeAttackMontage(this);
			}
		}
	}
}

void AATBPlayerCharacter::ServerRPCMeleeAttackEnd_Implementation()
{
	bCanAttack = true;
	OnRep_CanAttack();
}

void AATBPlayerCharacter::ClientRPCPlayMeleeAttackMontage_Implementation(AATBPlayerCharacter* InTargetCharacter)
{
	if (IsValid(InTargetCharacter))
	{
		InTargetCharacter->PlayAnimMontage(MeleeAttackMontage);
	}
}

void AATBPlayerCharacter::ServerRPCPerformMeleeHit_Implementation(AATBPlayerCharacter* InDamagedCharacter)
{
	if (IsValid(InDamagedCharacter) == true)
	{
		const float MeleeAttackDamage = 30.f;
		FDamageEvent DamageEvent;
		InDamagedCharacter->TakeDamage(MeleeAttackDamage, DamageEvent, GetController(), this);
	}
}

void AATBPlayerCharacter::ServerRPCDeathProcessing_Implementation()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this))
		{
			if (AATBGameModeBase* ATBGM = Cast<AATBGameModeBase>(GM))
			{
				if (AATBPlayerController* ATBPC = Cast<AATBPlayerController>(PC))
				{
					ATBPC->UnPossess();
					ATBGM->PlayerDead(ATBPC);
				}
			}
		}
	}
}

void AATBPlayerCharacter::DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
	const float MeleeAttackRange = 50.f;
	const float MeleeAttackRadius = 50.f;
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = MeleeAttackRange * 0.5f;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, MeleeAttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
}
