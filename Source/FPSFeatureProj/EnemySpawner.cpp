// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "EnemyManager.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemySpawner::SpawnEnemy(TSubclassOf<class AEnemy> EnemyToSpawnClass)
{
	if (!IsValid(EnemyToSpawnClass))
	{
		return;
	}

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemyToSpawnClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);
	Cast<AEnemy>(SpawnedEnemy)->SetEnemyManager(EnemyManager);
	EnemyManager->IncrementNumAliveEnemies();
}

void AEnemySpawner::SpawnDefaultEnemy()
{
	if (!IsValid(DefaultEnemyToSpawnClass))
	{
		return;
	}

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(DefaultEnemyToSpawnClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);
	Cast<AEnemy>(SpawnedEnemy)->SetEnemyManager(EnemyManager);
	EnemyManager->IncrementNumAliveEnemies();
}
