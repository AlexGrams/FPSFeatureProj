// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class FPSFEATUREPROJ_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// --- Functions ---

	// Spawn one Enemy. Will spawn regardless of blocking objects.
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnEnemy();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// --- Variables ---

	// Enemy that this spawner spawns
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawning")
	UClass* EnemyToSpawn;

};
