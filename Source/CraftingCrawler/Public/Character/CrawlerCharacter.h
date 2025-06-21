// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "CrawlerCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToolTipTextUpdated, FString, NewTooltip);

struct FInputActionValue;

UCLASS()
class CRAFTINGCRAWLER_API ACrawlerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACrawlerCharacter();
	TObjectPtr<UHealthComponent> GetHealthComponent() const
	{
		return HealthComponent;
	}
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& InputActionValue);
	void InteractWithClosestInteractable();
	AActor* GetClosestInteractable();
	
	void ApplyDamageInRange(const float DamageAmount, const float Range);
	
	void AttackPrimary();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	FString CurrentInteractionTooltipText = "";
	UPROPERTY(BlueprintAssignable)
	FOnToolTipTextUpdated OnToolTipTextUpdated;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> AttackCone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> PrimaryAttackMontage;
	
};
