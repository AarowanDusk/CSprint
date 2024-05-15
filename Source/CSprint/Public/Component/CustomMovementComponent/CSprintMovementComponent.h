// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CSprintMovementComponent.generated.h"

/**
 * 
 */

class APlayerCSprintCharacter;
class UParticleSystemComponent;
class UParticleSystem;

UCLASS()
class CSPRINT_API UCSprintMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UCSprintMovementComponent();

private:

#pragma region ReplicationLogic

	//SavedMoves
	class FSavedMove_C : public FSavedMove_Character
	{

		//Flags
		uint8 Saved_bCPressedJump : 1;
		uint8 Saved_bWantsToSprint : 1;
		uint8 Saved_bWantsToSprintBoost : 1;
		uint8 Saved_bFX : 1;

		typedef FSavedMove_Character Super;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* Character);

	};

	//NetworkPrediction
	class FNetworkPredictionData_Client_C : public FNetworkPredictionData_Client_Character
	{

	public:

		FNetworkPredictionData_Client_C(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;

	};

public:

	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	//ProxyReplications
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;

	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

protected:

	//FlagUpdates
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnClientCorrectionReceived(class FNetworkPredictionData_Client_Character& ClientData, float TimeStamp, FVector NewLocation, FVector NewVelocity, UPrimitiveComponent* NewBase, FName NewBaseBoneName, bool bHasBase, bool bBaseRelativePosition, uint8 ServerMovementMode, FVector ServerGravityDirection) override;
	virtual void InitializeComponent() override;

	virtual bool ServerCheckClientError(float ClientTimeStamp, float DeltaTime, const FVector& Accel, const FVector& ClientWorldLocation, const FVector& RelativeClientLocation, UPrimitiveComponent* ClientMovementBase, FName ClientBaseBoneName, uint8 ClientMovementMode) override;

	virtual void CallServerMovePacked(const FSavedMove_Character* NewMove, const FSavedMove_Character* PendingMove, const FSavedMove_Character* OldMove) override;

	FNetBitWriter CServerMoveBitWriter;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

private:

	UPROPERTY(Transient) APlayerCSprintCharacter* CCharacterOwner;

	float AccumulatedClientLocationError = 0.f;

	int TickCount = 0;
	int CorrectionCount = 0;
	int TotalBitsSent = 0;


#pragma region SprintVariable

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Sprint_Boost;
	UPROPERTY(EditDefaultsOnly) float Sprint_BoostTimer = 2;

	bool Safe_bWantsToSprint;
	bool Safe_bWantsToSprintBoost;

protected:

	UPROPERTY(BlueprintReadWrite) float Sprint_BoostCount;

#pragma endregion

public:

#pragma region SprintFunction

	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();
	UFUNCTION(BlueprintCallable) void SprintBoost();

	void ResetSafeWantsToSprintBoost();
	void BoostTimer();

#pragma endregion

};
