// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeaponComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

UEnemyWeaponComponent::UEnemyWeaponComponent()
{
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UEnemyWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(this->GetOwner()->GetParentActor());
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

//void UEnemyWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
void UEnemyWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Continuously look at Player
	// TODO: Clean up, one line
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = PlayerCharacter->GetActorLocation();
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
	this->GetOwner()->SetActorRotation(LookRotation);
}

void UEnemyWeaponComponent::TestPrint()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Testing print: UEnemyWeaponComponent")));
	//if (IsComponentTickEnabled())
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("This component can tick")));
	if (IsRegistered())
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Component is registered")));
}

FRotator UEnemyWeaponComponent::GetProjectileRotation() const
{
	return GetOwner()->GetActorRotation();
}