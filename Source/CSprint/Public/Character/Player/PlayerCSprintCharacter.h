// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/CSprintCharacter.h"
#include "PlayerCSprintCharacter.generated.h"

/**
 * 
 */

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCSprintMovementComponent;

UCLASS()
class CSPRINT_API APlayerCSprintCharacter : public ACSprintCharacter
{
	GENERATED_BODY()
	
public:

	bool bCPressedJump;

	APlayerCSprintCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void Jump() override;

	virtual void StopJumping() override;

	FCollisionQueryParams GetIgnoreCharacterParams() const;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE UCSprintMovementComponent* GetCharacterMovementComponent() const { return CSprintMovement; }

protected:

	virtual void Tick(float DeltaSeconds) override;

private:

	UPROPERTY(EditAnywhere, category = "Input")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, category = "Input")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, category = "Input")
	TObjectPtr<UCSprintMovementComponent> CSprintMovement;

	UPROPERTY()
	FRotator PreviousRotation = this->GetActorRotation();

public:

	void GetSprint();
	void GetSprintRelease();
	void GetBoost();

};
