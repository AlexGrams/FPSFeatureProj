// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponComponent.h"
#include "EnemyWeaponComponent.generated.h"


// Weapon component for Enemies
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPSFEATUREPROJ_API UEnemyWeaponComponent : public UBaseWeaponComponent
{
	GENERATED_BODY()

public:
	// --- Variables ---

	// TODO: Testing component variables.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Testing)
	int TestValue;

	// --- Functions ---

	// Sets up default values
	UEnemyWeaponComponent();

	UFUNCTION()
	void BeginPlay();

	// Tick function
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// TODO: Testing function
	void TestPrint();
	
protected:
	// --- Functions ---

	// Get spawn rotation for a projectile: forward direction of Weapon model
	virtual FRotator GetProjectileRotation() const override;
};
