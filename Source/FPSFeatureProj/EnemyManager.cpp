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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("TODO: Testing: Number of TempEnemySpawners = %d"), TempEnemySpawners.Num()));

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("TODO: Testing: Number of EnemySpawners = %d"), EnemySpawners.Num()));
	for (AActor* Actor : TempEnemySpawners)
	{
		AEnemySpawner* SpawnerToAdd = Cast<AEnemySpawner>(Actor);
		EnemySpawners.Add(SpawnerToAdd);
		SpawnerToAdd->SetEnemyManager(this);
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("TODO: Testing: Number of EnemySpawners = %d"), EnemySpawners.Num()));
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

