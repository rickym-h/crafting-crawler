// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	InitHealthComponent(3);
}

void UHealthComponent::OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy,
	AActor* DamageCauser)
{
	// Damage particle effects here?
	
	Health -= static_cast<int32>(Damage);
	if (Health <= 0)
	{
		GetOwner()->Destroy();
		// Dying particle effects here?
	}
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddUniqueDynamic(this, &UHealthComponent::OnOwnerTakeDamage);
	
}

void UHealthComponent::InitHealthComponent(const int32 InMaxHealth)
{
	MaxHealth = InMaxHealth;
	Health = MaxHealth;
}
