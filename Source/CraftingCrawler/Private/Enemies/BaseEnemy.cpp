// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemy.h"

#include "Character/CrawlerCharacter.h"
#include "Character/Components/HealthComponent.h"
#include "Core/CrawlerGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");

	AttackCone = CreateDefaultSubobject<UStaticMeshComponent>("Attack Cone");
	AttackCone->SetupAttachment(GetMesh());
	AttackCone->SetCollisionResponseToAllChannels(ECR_Overlap);
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (UCrawlerGameInstance* GameInstance = CastChecked<UCrawlerGameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstance->IncrementEnemyCount();
		HealthComponent->InitHealthComponent(FMath::Max(1, GameInstance->GetDungeonDepth()/2));
		AttackRange *= (1+(0.1 * GameInstance->GetDungeonDepth()));
		AttackPlayRate = (1+(0.1 * GameInstance->GetDungeonDepth()));
		NormalSpeed *= (1+(0.1 * GameInstance->GetDungeonDepth()));
		ChaseSpeed *= (1+(0.1 * GameInstance->GetDungeonDepth()));
	}
	AttackCone->SetWorldScale3D(FVector(AttackRange));
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &ABaseEnemy::HandleOnAttackMontageComplete);
	}
}

void ABaseEnemy::Destroyed()
{
	Super::Destroyed();
	
	if (UCrawlerGameInstance* GameInstance = Cast<UCrawlerGameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstance->DecrementEnemyCount();
	}
}

void ABaseEnemy::HandleOnAttackMontageComplete(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABaseEnemy::AttackPrimary()
{
	if (bIsAttacking) return true;
	bIsAttacking = true;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (PrimaryAttackMontage && AnimInstance && !AnimInstance->Montage_IsPlaying(PrimaryAttackMontage))
	{
		AnimInstance->Montage_Play(PrimaryAttackMontage, AttackPlayRate);
	}
	return true;
}

void ABaseEnemy::DealDamage()
{
	AttackCone->SetWorldScale3D(FVector(AttackRange));
	TArray<AActor*> HitActors;
	AttackCone->GetOverlappingActors(HitActors, ACrawlerCharacter::StaticClass());

	for (const auto Enemy : HitActors)
	{		
		UGameplayStatics::ApplyDamage(Enemy, DamageAmount, GetController(), this, UDamageType::StaticClass());
	}
}

void ABaseEnemy::BeginChasing()
{
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed = ChaseSpeed;
}

void ABaseEnemy::StopChasing()
{
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed = NormalSpeed;
}

