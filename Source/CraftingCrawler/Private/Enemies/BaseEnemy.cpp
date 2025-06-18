// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemy.h"

#include "Character/CrawlerCharacter.h"
#include "Character/Components/HealthComponent.h"
#include "Core/CrawlerGameInstance.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");

	AttackCone = CreateDefaultSubobject<UStaticMeshComponent>("Attack Cone");
	AttackCone->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->InitHealthComponent(1);

	if (UCrawlerGameInstance* GameInstance = CastChecked<UCrawlerGameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstance->IncrementEnemyCount();
	}

	AttackCone->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ABaseEnemy::Destroyed()
{
	Super::Destroyed();
	
	if (UCrawlerGameInstance* GameInstance = Cast<UCrawlerGameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstance->DecrementEnemyCount();
	}
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::AttackPrimary()
{
	bIsAttacking = true;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (PrimaryAttackMontage && AnimInstance && !AnimInstance->Montage_IsPlaying(PrimaryAttackMontage))
	{
		AnimInstance->Montage_Play(PrimaryAttackMontage);
	}
}

void ABaseEnemy::DealDamage()
{	
	TArray<AActor*> HitActors;
	AttackCone->GetOverlappingActors(HitActors, ACrawlerCharacter::StaticClass());

	for (const auto Enemy : HitActors)
	{		
		UGameplayStatics::ApplyDamage(Enemy, DamageAmount, GetController(), this, UDamageType::StaticClass());
	}
}

