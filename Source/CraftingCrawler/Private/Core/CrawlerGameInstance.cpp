// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CrawlerGameInstance.h"

UCrawlerGameInstance::UCrawlerGameInstance(): DungeonDepth(1), Gold(0), Attack(1), Defence(0), HealthPotCount(0), HealthPotEfficacy(1), StaminaPotCount(0), StaminaPotEfficacy(1)
{
	OnGoldChangedDelegate.Broadcast(Gold);
}

int32 UCrawlerGameInstance::GetGold() const
{
	return Gold;
}

bool UCrawlerGameInstance::SpendGold(const int32 Cost)
{
	if (Gold >= Cost)
	{
		Gold -= Cost;
		OnGoldChangedDelegate.Broadcast(Gold);
		return true;
	}
	return false;
}

int32 UCrawlerGameInstance::AddGold(const int32 GoldToAdd)
{
	Gold += GoldToAdd;
	OnGoldChangedDelegate.Broadcast(Gold);
	return Gold;
}

int32 UCrawlerGameInstance::GetAttack() const
{
	return Attack;
}

int32 UCrawlerGameInstance::GetDefence() const
{
	return Defence;
}
