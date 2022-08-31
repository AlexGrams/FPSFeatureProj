// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"
#include "Components/BoxComponent.h"
#include "FPSFeatureProjCharacter.h"

ARoomManager::ARoomManager()
{
	RoomCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomCollider"));
}

void ARoomManager::BeginPlay()
{
	PlayerHasEntered = false;
}

void ARoomManager::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	if (!PlayerHasEntered && OtherActor->IsA(AFPSFeatureProjCharacter::StaticClass()))
	{
		// TODO: Testing override entering
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Player has entered")));

		PlayerHasEntered = true;
		StartWave();
	}
}

void ARoomManager::DecrementNumAliveEnemies()
{
	Super::DecrementNumAliveEnemies();

	if (NumAliveEnemies <= 0)
	{
		// TODO: Open Doors.
	}
}