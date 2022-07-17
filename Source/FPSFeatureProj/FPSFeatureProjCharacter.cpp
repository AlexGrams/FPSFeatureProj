// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSFeatureProjCharacter.h"
#include "FPSFeatureProjProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"


//////////////////////////////////////////////////////////////////////////
// AFPSFeatureProjCharacter

AFPSFeatureProjCharacter::AFPSFeatureProjCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

void AFPSFeatureProjCharacter::PickUpWeapon(AActor* NewWeapon)
{
	Weapons.Add(NewWeapon);
	CurrentWeaponIndex = Weapons.Num() - 1;
	SwapToWeapon(CurrentWeaponIndex);
}

void AFPSFeatureProjCharacter::AddWeapon(AActor* NewWeapon)
{
	Weapons.Add(NewWeapon);
}

void AFPSFeatureProjCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////// Input

void AFPSFeatureProjCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AFPSFeatureProjCharacter::OnPrimaryAction);

	// Bind reload event
	PlayerInputComponent->BindAction("ReloadAction", IE_Pressed, this, &AFPSFeatureProjCharacter::OnReloadAction);

	// Bind weapon switching events
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &AFPSFeatureProjCharacter::OnNextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, this, &AFPSFeatureProjCharacter::OnPreviousWeapon);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFPSFeatureProjCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFPSFeatureProjCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AFPSFeatureProjCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AFPSFeatureProjCharacter::LookUpAtRate);
}

void AFPSFeatureProjCharacter::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnUseItem.Broadcast();
}

void AFPSFeatureProjCharacter::OnReloadAction()
{
	// Trigger OnReload Event
	OnReload.Broadcast();
}

void AFPSFeatureProjCharacter::OnNextWeapon()
{
	if (Weapons.Num() > 1)
	{
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
		SwapToWeapon(CurrentWeaponIndex);
	}
}

void AFPSFeatureProjCharacter::OnPreviousWeapon()
{
	if (Weapons.Num() > 1)
	{
		CurrentWeaponIndex = (CurrentWeaponIndex - 1 + Weapons.Num()) % Weapons.Num();
		SwapToWeapon(CurrentWeaponIndex);
	}
}

void AFPSFeatureProjCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AFPSFeatureProjCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AFPSFeatureProjCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSFeatureProjCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSFeatureProjCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFPSFeatureProjCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool AFPSFeatureProjCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFPSFeatureProjCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFPSFeatureProjCharacter::EndTouch);

		return true;
	}
	
	return false;
}

// Unequip previous weapon, equip new weapon
void AFPSFeatureProjCharacter::SwapToWeapon(UTP_WeaponComponent* NewEquipedWeapon)
{
	if (NewEquipedWeapon != nullptr)
	{
		if (CurrentWeapon != nullptr)
		{
			CurrentWeapon->Unequip();
		}

		CurrentWeapon = NewEquipedWeapon;
		CurrentWeapon->Equip();
	}
}

// Weapon swap wrapper requiring only weapon index
void AFPSFeatureProjCharacter::SwapToWeapon(int WeaponIndex)
{
	if (WeaponIndex > -1 && WeaponIndex < Weapons.Num())
	{
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Swapping to weapon %d"), WeaponIndex));
		SwapToWeapon(Cast<UTP_WeaponComponent>(Weapons[WeaponIndex]->GetComponentByClass(UTP_WeaponComponent::StaticClass())));
	}
}
