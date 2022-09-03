// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "EnemySpawner.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Get all EnemySpawners once so that they can be acced without recalculation later.
	TArray<AActor*> TempEnemySpawners;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), TempEnemySpawners);
	for (AActor* Actor : TempEnemySpawners)
	{
		AEnemySpawner* SpawnerToAdd = Cast<AEnemySpawner>(Actor);
		EnemySpawners.Add(SpawnerToAdd);
		SpawnerToAdd->SetEnemyManager(this);
	}
}

void AEnemyManager::StartWave()
{
	if (EnemySpawners.Num() <= 0)
	{
		return;
	}

	// Spawn one Enemy at each EnemySpawner until all wave Enemies have been spawned. Spawn multiple at the same EnemySpawner if there are more Enemies than Spawners.
	int EnemySpawnerIndex = 0;

	for (FWaveEnemies Wave : WaveEnemies)
	{
		if (!IsValid(Wave.Enemy))
		{
			continue;
		}

		for (int i = 0; i < Wave.Count; i++)
		{
			if (EnemySpawnerIndex >= EnemySpawners.Num())
			{
				EnemySpawnerIndex = 0;
			}

			EnemySpawners[EnemySpawnerIndex++]->SpawnEnemy(Wave.Enemy, this);
		}
	}
}

void AEnemyManager::IncrementNumAliveEnemies()
{
	NumAliveEnemies += 1;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("TODO: Testing: Number of alive enemies = %d"), NumAliveEnemies));
}

void AEnemyManager::DecrementNumAliveEnemies()
{
	NumAliveEnemies -= 1;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("TODO: Testing: Number of alive enemies = %d"), NumAliveEnemies));

	if (NumAliveEnemies <= 0)
	{
		// TODO: Game win state
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, FString::Printf(TEXT("You're winner!")));
	}
}

