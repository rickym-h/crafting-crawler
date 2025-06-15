// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingCrawler/Public/Character/CrawlerCharacter.h"

#include "InputActionValue.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


// Sets default values
ACrawlerCharacter::ACrawlerCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("CollisionCapsule");
	SetRootComponent(CollisionCapsule);
	CollisionCapsule->SetCapsuleSize(42.0f, 80.0f);
	CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionCapsule->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1000.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->InitHealthComponent(3);
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
	SkeletalMesh->SetRelativeRotation(Direction);
}

void ACrawlerCharacter::AttackPrimary()
{
	bIsAttacking = true;
	
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if (PrimaryAttackMontage && AnimInstance && !AnimInstance->Montage_IsPlaying(PrimaryAttackMontage))
	{
		AnimInstance->Montage_Play(PrimaryAttackMontage);
	}
}
