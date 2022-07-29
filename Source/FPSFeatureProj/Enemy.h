// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "Engine/EngineTypes.h"
#include "EnemyWeaponComponent.h"
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

	// ChildActorComponents that are this Enemy's weapons
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	TArray<FComponentReference> WeaponChildActors;

	// References to weapon Actors
	TArray<AActor*> Weapons;

	// References to EnemyWeaponComponents
	TArray<UEnemyWeaponComponent*> WeaponComponents;

	// TODO: Testing stuff things with component communication
	bool bHasFired;

	// --- Functions ---

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
