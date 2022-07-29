// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeaponComponent.h"

UEnemyWeaponComponent::UEnemyWeaponComponent()
{

}

void UEnemyWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(this->GetOwner()->GetParentActor());
}

void UEnemyWeaponComponent::TestPrint()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Testing print: UEnemyWeaponComponent")));
}