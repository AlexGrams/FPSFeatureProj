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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("TODO: Testing: Number of EnemySpawners = %d"), TempEnemySpawners.Num()));
}

