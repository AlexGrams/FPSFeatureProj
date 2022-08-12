// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

// Forward declaration
class AEnemyManager;

UCLASS()
class FPSFEATUREPROJ_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	// --- Functions ---
	
	// Spawns one of the specified Enemy. Will spawn regardless of blocking objects.
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnEnemy(TSubclassOf<class AEnemy> EnemyToSpawnClass);

	// Spawn one default Enemy. 
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnDefaultEnemy();

protected:
	// --- Variables ---

	// Pointer to this level's EnemyManager
	AEnemyManager* EnemyManager;
	
	// --- Functions ---
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// --- Variables ---

	// Enemy that this spawner spawns
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawning")
	TSubclassOf<class AActor> DefaultEnemyToSpawnClass;

	// --- Functions
	
	virtual void SetEnemyManager(AEnemyManager* NewEnemyManager) { EnemyManager = NewEnemyManager; }

};
