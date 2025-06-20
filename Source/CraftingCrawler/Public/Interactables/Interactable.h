// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRAFTINGCRAWLER_API IInteractable
{
	GENERATED_BODY()

public:
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	bool Interact();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	FString InteractToolTip();
};
