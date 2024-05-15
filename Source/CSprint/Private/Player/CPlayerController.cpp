// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Character/Player/PlayerCSprintCharacter.h"

ACPlayerController::ACPlayerController()
{

	bReplicates = true;

}

void ACPlayerController::BeginPlay()
{

	Super::BeginPlay();

	check(ADMContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(ADMContext, 0);
	}

	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Hand;

}

void ACPlayerController::SetupInputComponent()
{

	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPlayerController::Move);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACPlayerController::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACPlayerController::StopJumping);

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPlayerController::Look);

	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ACPlayerController::Sprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACPlayerController::SprintRelease);

	EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Triggered, this, &ACPlayerController::Boost);

}

void ACPlayerController::Move(const FInputActionValue& InputActionValue)
{

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDrection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDrection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{

		ControlledPawn->AddMovementInput(ForwardDrection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDrection, InputAxisVector.X);

	}

}

void ACPlayerController::Jump()
{

	if (GetCharacter())
	{

		GetCharacter()->Jump();

	}

}

void ACPlayerController::StopJumping()
{

	if (GetCharacter())
	{

		GetCharacter()->StopJumping();

	}

}

void ACPlayerController::Look(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{

		// add yaw and pitch input to controller
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);

	}
}

void ACPlayerController::Sprint(const FInputActionValue& InputActionValue)
{

	if (GetCharacter())
	{


		if (CPlayer())
		{

			CPlayer()->GetSprint();

			UE_LOG(LogTemp, Warning, TEXT("Sprinting!"));

		}

	}

}

void ACPlayerController::SprintRelease(const FInputActionValue& InputActionValue)
{

	if (GetCharacter())
	{

		if (CPlayer())
		{

			CPlayer()->GetSprintRelease();

			UE_LOG(LogTemp, Warning, TEXT("Sprinting Stopped!"));

		}

	}

}

void ACPlayerController::Boost(const FInputActionValue& InputActionValue)
{

	if (GetCharacter())
	{

		if (CPlayer())
		{

			CPlayer()->GetBoost();

			UE_LOG(LogTemp, Warning, TEXT("Boosting!"));

		}

	}

}

TObjectPtr<APlayerCSprintCharacter> ACPlayerController::CPlayer()
{

	Character = Cast<APlayerCSprintCharacter>(GetPawn());

	return Character;
}

