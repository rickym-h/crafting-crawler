// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingCrawler/Public/Core/Player/CrawlerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CraftingCrawler/Public/Character/CrawlerCharacter.h"


// Sets default values
ACrawlerController::ACrawlerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACrawlerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACrawlerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACrawlerController::Move);
		EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Triggered, this, &ACrawlerController::Primary);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACrawlerController::Interact);
	}
}

void ACrawlerController::Move(const FInputActionValue& Value)
{
	if (ACrawlerCharacter* CrawlerCharacter = Cast<ACrawlerCharacter>(GetPawn()))
	{
		CrawlerCharacter->Move(Value);
	}
}

void ACrawlerController::Primary(const FInputActionValue& Value)
{
	// If in combat mode - attack
	if (ACrawlerCharacter* CrawlerCharacter = Cast<ACrawlerCharacter>(GetPawn()))
	{
		CrawlerCharacter->AttackPrimary();
	}
	// TODO Otherwise - interact 
}

void ACrawlerController::Interact(const FInputActionValue& Value)
{
	if (ACrawlerCharacter* CrawlerCharacter = Cast<ACrawlerCharacter>(GetPawn()))
	{
		CrawlerCharacter->InteractWithClosestInteractable();
	}
}

// Called every frame
void ACrawlerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

