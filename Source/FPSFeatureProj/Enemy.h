// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "Enemy.generated.h"

class UHealthComponent;

UCLASS()
class FPSFEATUREPROJ_API AEnemy : public ACharacter
{
	GENERATED_BODY()

private:
	// Health component for taking damage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;

public:
	// Sets default values for this character's properties
	AEnemy();

	// Returns HealthComponent subobject
	FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	// --- Variables ---

	/*
	* TODO: Enemy architecture
	* Enemies have an array of soft class references to the BP weapons that they hold.
	* Also have array of "Attachment Points" (Scene) where the weapons go.
	* Weapons are spawned during BeginPlay.
	* Weapons are then attached to their corresponding Attachment Points.
	*/

	// Array of Blueprints for weapons this Enemy holds
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	TArray<TSoftClassPtr<AActor>> WeaponClasses;

	// Locations for each corresponding weapon, ie. Index 0 = location of 0th weapon.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	TArray<USceneComponent*> WeaponAttachmentPoints;

	// Pointers to instantiated weapons
	TArray<AActor*> Weapons;

	// --- Functions ---

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
