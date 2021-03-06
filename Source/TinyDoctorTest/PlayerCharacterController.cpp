// Fill out your copyright notice in the Description page of Project Settings.

#include "TinyDoctorTest.h"
#include "PlayerCharacterController.h"

#include "PlayerCharacter.h"
#include "StandardGameMode.h"

// Holds reference to the ACharacter the class is controlling.
APlayerCharacter* playerReference;

// Decides how much you need to move the Gamepad stick to activate the gamepad.
constexpr float gamepadActivateThreshold = 0.25f;
FVector gamepadAimDirection = FVector::ZeroVector;
// Holds the position of the mousecursor. Tests if the mouse has moved to see whether to activate mouse-aiming.
FVector previousMousePosition = FVector::ZeroVector;

APlayerCharacterController::APlayerCharacterController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;
}

// Called to bind functionality to input
void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Movement bindings
	InputComponent->BindAxis("MoveForward", this, &APlayerCharacterController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacterController::MoveRight);
	InputComponent->BindAxis("GamepadAimForward", this, &APlayerCharacterController::GamepadAimForward);
	InputComponent->BindAxis("GamepadAimRight", this, &APlayerCharacterController::GamepadAimRight);
	
	// Actions binding
	InputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerCharacterController::Shoot);
	InputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCharacterController::PauseGame).bExecuteWhenPaused = true;

	InputComponent->BindAction("Choose Standard Weapon", IE_Pressed, this, &APlayerCharacterController::SetRifleActive);
	InputComponent->BindAction("Choose Shotgun", IE_Pressed, this, &APlayerCharacterController::SetShotgunActive);
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	playerReference = Cast<APlayerCharacter>(GetCharacter());
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	if (bUsingGamepad && CheckForMouseMovement() == true)
	{
		bUsingGamepad = false;
		bShowMouseCursor = true;
	}

	if (!bUsingGamepad)
		OrientTowardsCursor();
	else
		OrientTowardsGamepadAnalog();
}

void APlayerCharacterController::SetShotgunActive()
{
	if (playerReference->IsRifleActive())
	{
		playerReference->bRifleActive = false;
		playerReference->WeaponChangeEvent();
	}
}

void APlayerCharacterController::SetRifleActive()
{
	if (!playerReference->IsRifleActive())
	{
		playerReference->bRifleActive = true;
		playerReference->WeaponChangeEvent();
	}
}

void APlayerCharacterController::PauseGame()
{
	Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode())->PauseGame();
}

void APlayerCharacterController::MoveForward(const float value)
{
	playerReference->MoveForward(value);
}

void APlayerCharacterController::MoveRight(const float value)
{
	playerReference->MoveRight(value);
}

void APlayerCharacterController::Shoot()
{
	playerReference->Shoot();	
}

const bool APlayerCharacterController::CheckForMouseMovement()
{
	FVector currentMousePosition;
	GetMousePosition(currentMousePosition.X, currentMousePosition.Y);
	if (currentMousePosition != previousMousePosition)
		return true;
	return false;
}

void APlayerCharacterController::OrientTowardsCursor()
{
	// Class that holds information about the object it hit.
	FHitResult Hit;
	// Ray-traces to the object under the cursor and places it in the Hit-variable above.
	if (GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit))
	{
		// Calculates the vector between the actor and the point of impact caused by the ray-trace
		FVector targetVector = Hit.ImpactPoint - playerReference->GetActorLocation();

		// Rotates the player accordingly.
		ControlRotation.Yaw = targetVector.Rotation().Yaw;
	}
}

void APlayerCharacterController::GamepadAimForward(const float value)
{
	if (bUsingGamepad)
		gamepadAimDirection.X = value;
	else
	{
		// Checks whether to activate controller-aiming
		if (value >= gamepadActivateThreshold)
			ActivateGamepad();
	}
}

void APlayerCharacterController::GamepadAimRight(const float value)
{
	if (bUsingGamepad)
		gamepadAimDirection.Y = value;
	else
	{
		// Checks whether to activate controller-aiming
		if (value >= gamepadActivateThreshold)
			ActivateGamepad();
	}
}

void APlayerCharacterController::ActivateGamepad()
{
	bUsingGamepad = true;
	GetMousePosition(previousMousePosition.X, previousMousePosition.Y);
	bShowMouseCursor = false;
}

void APlayerCharacterController::OrientTowardsGamepadAnalog()
{
	constexpr float gamepadDeadzone = 0.5f;

	// Orients the player according to analog and camera-angle
	if (gamepadAimDirection.Size() >= gamepadDeadzone)
		ControlRotation.Yaw = gamepadAimDirection.Rotation().Yaw + 90.f + playerReference->springArm->GetComponentRotation().Yaw;
}