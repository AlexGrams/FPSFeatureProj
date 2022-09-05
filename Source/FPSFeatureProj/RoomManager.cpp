// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"
#include "Components/BoxComponent.h"
#include "FPSFeatureProjCharacter.h"
#include "ToggleableInterface.h"
#include "Kismet/KismetSystemLibrary.h"

ARoomManager::ARoomManager()
{
	RoomCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomCollider"));
}

void ARoomManager::BeginPlay()
{
	PlayerHasEntered = false;
	NumAliveEnemies = 0;
}

void ARoomManager::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Something entered the zone")));
	if (!PlayerHasEntered && OtherActor->IsA(AFPSFeatureProjCharacter::StaticClass()))
	{
		PlayerHasEntered = true;
		StartWave();

		// Close doors
		for (AActor* DoorActor : Doors)
		{
			if (UKismetSystemLibrary::DoesImplementInterface(DoorActor, UToggleableInterface::StaticClass()))
			{
				Cast<IToggleableInterface>(DoorActor)->Execute_Toggle(DoorActor, true);
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Closed a door")));
			}
		}
	}
}

void ARoomManager::DecrementNumAliveEnemies()
{
	Super::DecrementNumAliveEnemies();

	if (NumAliveEnemies <= 0)
	{
		// Open Doors.
		for (AActor* DoorActor : Doors)
		{
			if (UKismetSystemLibrary::DoesImplementInterface(DoorActor, UToggleableInterface::StaticClass()))
			{
				Cast<IToggleableInterface>(DoorActor)->Execute_Toggle(DoorActor, false);
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Opened a door")));
			}
		}
	}
}