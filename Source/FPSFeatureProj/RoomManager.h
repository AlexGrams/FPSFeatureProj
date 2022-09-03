// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyManager.h"
#include "RoomManager.generated.h"

class UBoxComponent;

/**
 * Manager class for a Room and its Enemies. Closes doors when Player first enters and spawns Enemies.
 * Opens doors once Enemies are defeated.
 */
UCLASS()
class FPSFEATUREPROJ_API ARoomManager : public AEnemyManager
{
	GENERATED_BODY()

public:
	// --- Variables ---

	// Doors that will be closed/opened by this Room. Must have ToggleableInterface.
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Room")
	TArray<AActor*> Doors;

	// --- Functions ---

	ARoomManager();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Special decrement for Rooms: if no Enemies left, open Doors.
	virtual void DecrementNumAliveEnemies() override;

protected:
	// --- Variables ---

	// Collider representing the inside of this room. Used to tell when Player enters room.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Room")
	UBoxComponent* RoomCollider;

	bool PlayerHasEntered;

	// --- Functions ---

	virtual void BeginPlay() override;
	
};
