// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingCrawler/Public/Character/CrawlerCharacter.h"

#include "InputActionValue.h"
#include "Core/CrawlerGameInstance.h"
#include "Enemies/BaseEnemy.h"
#include "Interactables/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ACrawlerCharacter::ACrawlerCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1000.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

	AttackCone = CreateDefaultSubobject<UStaticMeshComponent>("Attack Cone");
	AttackCone->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ACrawlerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UCrawlerGameInstance* GameInstance = Cast<UCrawlerGameInstance>(GetGameInstance()))
	{
		HealthComponent->InitHealthComponent(GameInstance->MaxHealth, GameInstance->CurrentHealth);
	}
	
}

// Called every frame
void ACrawlerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (AActor* ClosestInteractable = GetClosestInteractable())
	{
		FString NewCurrentInteractionTooltipText = IInteractable::Execute_InteractToolTip(ClosestInteractable);
		if (NewCurrentInteractionTooltipText != CurrentInteractionTooltipText)
		{
			CurrentInteractionTooltipText = NewCurrentInteractionTooltipText;
			OnToolTipTextUpdated.Broadcast(CurrentInteractionTooltipText);
		}
	} else
	{
		if (CurrentInteractionTooltipText != "")
		{
			CurrentInteractionTooltipText = "";
			OnToolTipTextUpdated.Broadcast(CurrentInteractionTooltipText);
		}
	}
}

// Called to bind functionality to input
void ACrawlerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACrawlerCharacter::Move(const FInputActionValue& InputActionValue)
{
	if (bIsAttacking) return;

	const FVector DirectionVector = InputActionValue.Get<FVector>().GetClampedToMaxSize(1.0f);
	if (!DirectionVector.IsNearlyZero())
	{
		AddMovementInput(DirectionVector);
	}

	FRotator Direction = DirectionVector.Rotation() + FRotator(0.0f, 270.0f, 0.0f);
	GetMesh()->SetRelativeRotation(Direction);
}

void ACrawlerCharacter::InteractWithClosestInteractable()
{
	// Interact with the closest interactable if found
    if (AActor* ClosestInteractable = GetClosestInteractable())
    {
        IInteractable::Execute_Interact(ClosestInteractable);
    }
}

AActor* ACrawlerCharacter::GetClosestInteractable()
{
	// Set up parameters for the sphere overlap
	constexpr float InteractionRadius = 100.0f;

	const FVector CharacterLocation = GetActorLocation();

	// Perform sphere overlap to find all actors within range
	TArray<UPrimitiveComponent*> OverlappingComponents;
	UKismetSystemLibrary::SphereOverlapComponents(
		GetWorld(),
		CharacterLocation,
		InteractionRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>(), // All object types
		UPrimitiveComponent::StaticClass(),
		TArray<AActor*>(), // Actors to ignore
		OverlappingComponents
	);

	// Find the closest interactable
	AActor* ClosestInteractable = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	for (UPrimitiveComponent* Component : OverlappingComponents)
	{
		// Check if actor implements IInteractable interface
		if (Component && Component->GetOwner()->Implements<UInteractable>())
		{
			const float Distance = FVector::Distance(CharacterLocation, Component->GetOwner()->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestInteractable = Component->GetOwner();
			}
		}
	}
	
	return ClosestInteractable;
}

void ACrawlerCharacter::ApplyDamageInRange(const float DamageAmount, const float Range)
{
	AttackCone->SetWorldScale3D(FVector(Range, Range, Range));
	
	TArray<AActor*> HitActors;
	AttackCone->GetOverlappingActors(HitActors, ABaseEnemy::StaticClass());

	for (const auto Enemy : HitActors)
	{		
		UGameplayStatics::ApplyDamage(Enemy, DamageAmount, GetController(), this, UDamageType::StaticClass());
	}
}

void ACrawlerCharacter::AttackPrimary()
{
	if (bIsAttacking) return;
	
	bIsAttacking = true;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (PrimaryAttackMontage && AnimInstance && !AnimInstance->Montage_IsPlaying(PrimaryAttackMontage))
	{
		AnimInstance->Montage_Play(PrimaryAttackMontage);
	}

	if (const UCrawlerGameInstance* GameInstance = Cast<UCrawlerGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		ApplyDamageInRange(GameInstance->GetAttack(), 1.5f);
	}
}