// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UHealthInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSFEATUREPROJ_API IHealthInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// --- Functions ---

	// Called when this object takes damage
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	void DamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	// Called when this object no longer has any health
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	void HealthDepleted();
};
