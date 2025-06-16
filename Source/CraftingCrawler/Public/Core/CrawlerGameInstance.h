// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CrawlerGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChangedSignature, int32, GoldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackChangedSignature, int32, AttackValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDefenceChangedSignature, int32, DefenceValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemiesChangedSignature, int32, EnemyCount);

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

	UPROPERTY(BlueprintAssignable)
	FOnGoldChangedSignature OnGoldChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnAttackChangedSignature OnAttackChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnDefenceChangedSignature OnDefenceChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnEnemiesChangedSignature OnEnemiesChangedDelegate;
	
	UFUNCTION(BlueprintCallable)
	int32 GetAttack() const;
	UFUNCTION(BlueprintCallable)
	int32 UpgradeAttack(const int32 AttackIncrement);
	UFUNCTION(BlueprintCallable)
	int32 GetDefence() const;
	UFUNCTION(BlueprintCallable)
	int32 UpgradeDefence(const int32 DefenceIncrement);
	
	UFUNCTION(BlueprintCallable)
	int32 GetEnemyCount() const;
	UFUNCTION(BlueprintCallable)
	int32 IncrementEnemyCount();
	UFUNCTION(BlueprintCallable)
	int32 DecrementEnemyCount();

protected:
	// Game
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 DungeonDepth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 EnemyCount;

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
