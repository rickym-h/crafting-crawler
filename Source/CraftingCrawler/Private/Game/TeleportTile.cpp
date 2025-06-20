// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/TeleportTile.h"

#include "Components/BoxComponent.h"
#include "Core/CrawlerGameInstance.h"


// Sets default values
ATeleportTile::ATeleportTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("TileMesh");
	SetRootComponent(TileMesh);
	
	OverlapTrigger = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	OverlapTrigger->SetupAttachment(TileMesh);
}

bool ATeleportTile::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ATeleportTile::Interact_Implementation()"));
	const UCrawlerGameInstance* GameInstance = Cast<UCrawlerGameInstance>(GetGameInstance());
	if (!GameInstance) return false;

	if (GameInstance->GetEnemyCount() <= 0)
	{
		// Can be activated
		OnInteractEvent();
	}
	
	return true;
}

FString ATeleportTile::InteractToolTip_Implementation()
{
	return "DEFAULT TEXT";
}

// Called when the game starts or when spawned
void ATeleportTile::BeginPlay()
{
	Super::BeginPlay();
	
}

