// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingCrawler/Public/Character/CrawlerCharacter.h"

#include "InputActionValue.h"
#include "Enemies/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ACrawlerCharacter::ACrawlerCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	// CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("CollisionCapsule");
	// SetRootComponent(CollisionCapsule);
	// CollisionCapsule->SetCapsuleSize(42.0f, 80.0f);
	// CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// CollisionCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	// CollisionCapsule->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1000.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

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
	GetMesh()->SetRelativeRotation(Direction);
}

void ACrawlerCharacter::ApplyDamageInRadius(const float DamageAmount, const float Radius)
{
	const FVector Origin = GetActorLocation();

	// Step 1: Sphere overlap to find nearby actors
	TArray<FHitResult> HitResults;
	const FVector SphereCenter = Origin;
	const float SphereRadius = Radius;

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		SphereCenter,
		SphereCenter,
		SphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		IgnoredActors,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);

	for (const FHitResult& Hit : HitResults)
	{
		AActor* Target = Hit.GetActor();
		if (!Target || Target == this) continue;
		
		UGameplayStatics::ApplyDamage(Target, DamageAmount, GetController(), this, UDamageType::StaticClass());
	}
}

void ACrawlerCharacter::AttackPrimary()
{
	bIsAttacking = true;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (PrimaryAttackMontage && AnimInstance && !AnimInstance->Montage_IsPlaying(PrimaryAttackMontage))
	{
		AnimInstance->Montage_Play(PrimaryAttackMontage);
	}

	ApplyDamageInRadius(1, 200);
}
