// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/ChildActorComponent.h"
#include "EnemyWeaponComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	* TODO: CAN BE DONE IN BLUEPRINTS I THINK
	* 
	* Load weapon classes asynchronously.
	* Add to Weapons TArray.
	* Child to corresponding WeaponAttachmentPoint.
	* Place at origin of parent (ie WeaponAttachmentPoint position).
	*/

	for (auto& Weapon : WeaponChildActors)
	{
		Weapons.Add(Cast<UChildActorComponent>(Weapon.GetComponent(this))->GetChildActor());
		WeaponComponents.Add(Cast<UEnemyWeaponComponent>(Weapons.Last()->GetComponentByClass(UEnemyWeaponComponent::StaticClass())));
	}

	// TODO: Testing what previous code does
	for (auto& Thing : Weapons)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Something was added %s"), *Thing->GetActorNameOrLabel()));
	}
	for (auto& Thing : WeaponComponents)
	{
		Thing->TestPrint();
		bHasFired = false;
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Testing weapon firing
	if (!bHasFired)
	{
		bHasFired = true;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Attempting omega firing")));

		// TODO: Disabled until solution found for missing fire parameters
		//WeaponComponents.Last()->Fire();
	}
}

