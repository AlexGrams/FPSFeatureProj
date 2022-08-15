// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_ShotgunComponent.h"
#include "Kismet/KismetMathLibrary.h"

UTP_ShotgunComponent::UTP_ShotgunComponent()
{
	// Shotgun weapon default settings
	NumPellets = 50;
	SpreadHalfAngleDegrees = 10.0f;
}

void UTP_ShotgunComponent::Fire()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Shotgun fired")));
	FVector PelletDirection = FVector::Zero();

	for (int i = 0; i < NumPellets; i++)
	{
		// FVector RandomUnitVectorInConeInDegrees(FVector ConeDir, float ConeHalfAngleInDegrees)
		PelletDirection = FMath::VRandCone(GetHitscanDirection(), FMath::DegreesToRadians(SpreadHalfAngleDegrees));
		//KismetMathLibrary::RandomUnitVectorInConeInDegrees(, SpreadHalfAngleDegrees);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), &PelletDirection.ToCompactString()));
	}
}