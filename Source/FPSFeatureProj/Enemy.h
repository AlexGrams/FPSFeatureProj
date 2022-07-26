// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "Enemy.generated.h"

class UHealthComponent;

UCLASS()
class FPSFEATUREPROJ_API AEnemy : public ACharacter
{
	GENERATED_BODY()

private:
	// Health component for taking damage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;

public:
	// Sets default values for this character's properties
	AEnemy();

	// Returns HealthComponent subobject
	FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
