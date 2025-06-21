// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class UHealthComponent;

UCLASS()
class CRAFTINGCRAWLER_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseEnemy();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> PrimaryAttackMontage;
	UFUNCTION()
	void HandleOnAttackMontageComplete(UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> AttackCone;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float NormalSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float ChaseSpeed = 300.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool AttackPrimary();
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DealDamage();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 DamageAmount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 1;
	float AttackPlayRate = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bIsAttacking = false;
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void BeginChasing();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopChasing();
};
