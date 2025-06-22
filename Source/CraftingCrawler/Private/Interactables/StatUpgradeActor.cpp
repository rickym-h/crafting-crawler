// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/StatUpgradeActor.h"

#include "Character/CrawlerCharacter.h"
#include "Components/BoxComponent.h"
#include "Core/CrawlerGameInstance.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AStatUpgradeActor::AStatUpgradeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	OverlapTrigger = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	SetRootComponent(OverlapTrigger);
}

// Called when the game starts or when spawned
void AStatUpgradeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AStatUpgradeActor::Interact_Implementation()
{
	return StatUpgradeImplementation();
}

FString AStatUpgradeActor::InteractToolTip_Implementation()
{
	return InteractText;
}

bool AStatUpgradeActor::UpgradeAttack(const int32 Amount)
{
	UCrawlerGameInstance* GameInstance = Cast<UCrawlerGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GameInstance) return false;

	if (GameInstance->SpendGold(UpgradeCost))
	{
		GameInstance->UpgradeAttack(Amount);
		return true;
	}
	return false;
}

bool AStatUpgradeActor::UpgradeDefence(const int32 Amount)
{
	UCrawlerGameInstance* GameInstance = Cast<UCrawlerGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GameInstance) return false;

	if (GameInstance->SpendGold(UpgradeCost))
	{
		GameInstance->UpgradeDefence(Amount);
		return true;
	}
	return false;
}

bool AStatUpgradeActor::UpgradeHealth(const int32 HealthIncrement)
{
	UCrawlerGameInstance* GameInstance = Cast<UCrawlerGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GameInstance) return false;

	ACrawlerCharacter* CrawlerCharacter = Cast<ACrawlerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!CrawlerCharacter) return false;
	
	if (GameInstance->SpendGold(UpgradeCost))
	{
		CrawlerCharacter->GetHealthComponent()->InitHealthComponent(CrawlerCharacter->GetHealthComponent()->GetMaxHealth() + HealthIncrement);
		return true;
	}
	return false;
}
