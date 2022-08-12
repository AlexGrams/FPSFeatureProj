// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

// Forward declaration
class AEnemySpawner;

UCLASS()
class FPSFEATUREPROJ_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// --- Functions ---

	// Decrease count of Enemies by one. Call when an Enemy is spawned.
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	void IncrementNumAliveEnemies();

	// Decrease count of Enemies by one. Call when an Enemy dies.
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	void DecrementNumAliveEnemies();

	// Sets default values for this actor's properties
	AEnemyManager();


protected:
	// --- Functions

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// --- Variables

	// All EnemySpawners in this level
	TArray<AEnemySpawner*> EnemySpawners;

	int NumAliveEnemies;
};
