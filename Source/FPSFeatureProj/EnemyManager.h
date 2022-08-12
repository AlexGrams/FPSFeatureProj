// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "EnemyManager.generated.h"

// Forward declaration
class AEnemySpawner;

// Contains class and count for each different Enemy that will be spawned per wave.
USTRUCT(BlueprintType)
struct FWaveEnemies
{
	GENERATED_BODY()

	// Blueprint class of an Enemy that will participate in this wave.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Enemies")
	TSubclassOf<AEnemy> Enemy;

	// How may of the specified Enemy class will be spawned this wave.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Enemies")
	int Count;
};

UCLASS()
class FPSFEATUREPROJ_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// --- Variables ---

	// List of class and count of all enemies that will be spawned this wave.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Waves")
	TArray<FWaveEnemies> WaveEnemies;

	// --- Functions ---

	// Starts a combat wave by spawning a collection of Enemies.
	UFUNCTION(BlueprintCallable, Category = "Waves")
	void StartWave();

	// Decrease count of Enemies by one. Call when an Enemy is spawned.
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	void IncrementNumAliveEnemies();

	// Decrease count of Enemies by one. Call when an Enemy dies.
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	void DecrementNumAliveEnemies();

	// Sets default values for this actor's properties
	AEnemyManager();


protected:
	// --- Functions ---

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// --- Variables ---

	// All EnemySpawners in this level
	TArray<AEnemySpawner*> EnemySpawners;

	int NumAliveEnemies;
};
