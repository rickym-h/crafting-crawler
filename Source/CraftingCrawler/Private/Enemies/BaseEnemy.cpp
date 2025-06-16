// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemy.h"

#include "Character/Components/HealthComponent.h"
#include "Core/CrawlerGameInstance.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
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
}

void ABaseEnemy::Destroyed()
{
	Super::Destroyed();
	
	if (UCrawlerGameInstance* GameInstance = CastChecked<UCrawlerGameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstance->DecrementEnemyCount();
	}
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

