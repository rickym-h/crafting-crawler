// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "StatUpgradeActor.generated.h"

class UBoxComponent;

UCLASS()
class CRAFTINGCRAWLER_API AStatUpgradeActor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStatUpgradeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> OverlapTrigger;

	virtual bool Interact_Implementation() override;
	virtual FString InteractToolTip_Implementation() override;
	UFUNCTION(BlueprintImplementableEvent)
	bool StatUpgradeImplementation();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Purchasing")
	FString InteractText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
	int32 UpgradeCost;

	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	bool UpgradeAttack(const int32 Amount) const;
	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	bool UpgradeDefence(const int32 Amount) const;
	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	bool UpgradeHealth(const int32 HealthIncrement) const;

};
