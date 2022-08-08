// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Engine/World.h"

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

void AEnemySpawner::SpawnEnemy()
{
	if (!IsValid(EnemyToSpawn))
	{
		return;
	}

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(EnemyToSpawn, NULL, NULL, true);
}

