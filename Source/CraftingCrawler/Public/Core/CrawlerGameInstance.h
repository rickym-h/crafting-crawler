// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CrawlerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGCRAWLER_API UCrawlerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UCrawlerGameInstance();
	
	UFUNCTION(BlueprintCallable)
	int32 GetGold() const;
	/**
	 * 
	 * @param Cost The amount of gold to spend
	 * @return true if successful, false if not enough gold
	*/
	UFUNCTION(BlueprintCallable)
	bool SpendGold(const int32 Cost);
	UFUNCTION(BlueprintCallable)
	int32 AddGold(const int32 GoldToAdd);
	UFUNCTION(BlueprintCallable)
	int32 GetAttack() const;
	UFUNCTION(BlueprintCallable)
	int32 GetDefence() const;

protected:

	// Stats
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Gold;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Attack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Defence;

	// Potions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HealthPotCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HealthPotEfficacy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 StaminaPotCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 StaminaPotEfficacy;
};
