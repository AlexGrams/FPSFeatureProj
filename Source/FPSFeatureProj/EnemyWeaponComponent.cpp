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

	// In order to tick, component must have PrimaryComponentTick.bCanEverTick = true and be registered.
	//bAutoActivate = true;
	//RegisterComponent();
	//SetComponentTickEnabled(true);
	//RegisterComponentTickFunctions(true);

	// TODO: Works with all enabled
	//PrimaryComponentTick.Target = this;
	//PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.SetTickFunctionEnable(true);
	//RegisterComponent();
	//PrimaryComponentTick.RegisterTickFunction(GetComponentLevel());

	Character = Cast<ACharacter>(this->GetOwner()->GetParentActor());
}

//void UEnemyWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
void UEnemyWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// TODO: Testing stuff. works now
	//Fire();
	//TestPrint();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Enemy weapon ticking")));


	// Looks at player and shoots continuously if they are close
	if (FVector::Distance(GetOwner()->GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()) <= 100.0f)
	{
		// TODO: Clean up, one line
		FVector Start = GetOwner()->GetActorLocation();
		FVector End = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
		this->GetOwner()->SetActorRotation(LookRotation);

		// Fire continuously
		StartAutoFire();
	}
	else if (FireInputHeld)
	{
		EndAutoFire();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Player out of range")));
	}
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