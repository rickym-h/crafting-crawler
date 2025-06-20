// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, int32, NewHealthValue);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRAFTINGCRAWLER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();
	
  	void InitHealthComponent(const int32 InMaxHealth);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChangedDelegate;
	int32 GetHealth() const
	{
		return Health;
	}

	int32 GetMaxHealth() const
	{
		return MaxHealth;
	}

protected:
	UFUNCTION()
	void OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 Health;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 MaxHealth;
};
