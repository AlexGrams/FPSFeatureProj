// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ToggleableInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UToggleableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for objects that can toggle between two states.
 */
class FPSFEATUREPROJ_API IToggleableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Toggleable")
	void Toggle(bool bOn);
};
