// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSFeatureProjGameMode.h"
#include "FPSFeatureProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSFeatureProjGameMode::AFPSFeatureProjGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
