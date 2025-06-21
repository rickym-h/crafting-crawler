// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "TeleportTile.generated.h"

class UBoxComponent;

UCLASS()
class CRAFTINGCRAWLER_API ATeleportTile : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeleportTile();

	virtual bool Interact_Implementation() override;

	virtual FString InteractToolTip_Implementation() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractEvent();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> OverlapTrigger;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FString InteractText;
};
