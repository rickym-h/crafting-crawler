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
	HealthComponent->InitHealthComponent(3);

	AttackCone = CreateDefaultSubobject<UStaticMeshComponent>("Attack Cone");
	AttackCone->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ACrawlerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrawlerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
    // Set up parameters for the sphere overlap
    constexpr float InteractionRadius = 200.0f;
    const FVector CharacterLocation = GetActorLocation();
    
    // Perform sphere overlap to find all actors within range
    TArray<AActor*> OverlappingActors;
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        CharacterLocation,
        InteractionRadius,
        TArray<TEnumAsByte<EObjectTypeQuery>>(), // All object types
        AActor::StaticClass(),
        TArray<AActor*>(), // Actors to ignore
        OverlappingActors
    );

    // Find the closest interactable
    AActor* ClosestInteractable = nullptr;
    float ClosestDistance = InteractionRadius;

    for (AActor* Actor : OverlappingActors)
    {
        // Check if actor implements IInteractable interface
        if (Actor && Actor->Implements<UInteractable>())
        {
            const float Distance = FVector::Distance(CharacterLocation, Actor->GetActorLocation());
            if (Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestInteractable = Actor;
            }
        }
    }

    // Interact with the closest interactable if found
    if (ClosestInteractable)
    {
        IInteractable::Execute_Interact(ClosestInteractable);
    }
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