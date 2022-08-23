// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMeleeWeaponComponent.h"
#include "FPSFeatureProjCharacter.h"
#include "Components/ShapeComponent.h"


void UEnemyMeleeWeaponComponent::Fire()
{
	//void GetOverlappingActors
	//(
	//	TArray< AActor* > &OverlappingActors,
	//	TSubclassOf< AActor > ClassFilter
	//)
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Attempting firing")));

	if (!IsValid(Hitbox))
	{
		return;
	}

	TArray<AActor*> HitActors;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Checking for colliding actors")));

	// Get the Player if they are overlapping with the hitbox
	Hitbox->GetOverlappingActors(HitActors, AFPSFeatureProjCharacter::StaticClass());

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Hit Players: %d"), HitActors.Num()));
}
