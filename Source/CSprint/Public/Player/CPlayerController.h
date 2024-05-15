// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UInputAction;
class APlayerCSprintCharacter;

struct FInputActionValue;

UCLASS()
class CSPRINT_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ACPlayerController();

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditAnywhere, category = "Input")
	TObjectPtr<UInputMappingContext> ADMContext;

	UPROPERTY(EditAnywhere, category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, category = "Input Sprint")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, category = "Input Boost")
	TObjectPtr<UInputAction> BoostAction;

	APlayerCSprintCharacter* Character;

	UFUNCTION()
	void Move(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Jump();

	UFUNCTION()
	void StopJumping();

	UFUNCTION()
	void Look(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Sprint(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void SprintRelease(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Boost(const FInputActionValue& InputActionValue);

	TObjectPtr<APlayerCSprintCharacter> CPlayer();


};
