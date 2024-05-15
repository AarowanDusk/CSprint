// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CustomMovementComponent/CSprintMovementComponent.h"
#include "Character/Player/PlayerCSprintCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

void UCSprintMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickCount++;
	if (IsNetMode(NM_Client))
	{

		GEngine->AddOnScreenDebugMessage(2, 100.f, FColor::Yellow, FString::Printf(TEXT("Correction: %.2f"), 100.f * (float)CorrectionCount / (float)TickCount));
		GEngine->AddOnScreenDebugMessage(9, 100.f, FColor::Yellow, FString::Printf(TEXT("Bitrate: %.3f"), (float)TotalBitsSent / GetWorld()->GetTimeSeconds() / 1000.f));

	}
	else
	{

		GEngine->AddOnScreenDebugMessage(3, 100.f, FColor::Yellow, FString::Printf(TEXT("Location Error: %.4f cm/s"), 100.f * AccumulatedClientLocationError / GetWorld()->GetTimeSeconds()));

	}


	UE_LOG(LogTemp, Warning, TEXT("Current MaxWalkSpeed: %f"), MaxWalkSpeed);

}

UCSprintMovementComponent::UCSprintMovementComponent()
{

	NavAgentProps.bCanCrouch = true;
	CServerMoveBitWriter.SetAllowResize(true);

}

#pragma region ReplicationLogic

bool UCSprintMovementComponent::FSavedMove_C::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{

	FSavedMove_C* NewCMove = static_cast<FSavedMove_C*>(NewMove.Get());

	if (Saved_bWantsToSprint != NewCMove->Saved_bWantsToSprint)
	{
		return false;
	}

	if (Saved_bWantsToSprintBoost != NewCMove->Saved_bWantsToSprintBoost)
	{
		return false;
	}

	return CanCombineWith(NewMove, InCharacter, MaxDelta);

}

void UCSprintMovementComponent::FSavedMove_C::Clear()
{

	FSavedMove_Character::Clear();

	Saved_bCPressedJump = 0;
	Saved_bWantsToSprint = 0;
	Saved_bWantsToSprintBoost = 0;

}

uint8 UCSprintMovementComponent::FSavedMove_C::GetCompressedFlags() const
{

	uint8 CompressedFlags = Super::GetCompressedFlags();

	if (Saved_bCPressedJump) CompressedFlags |= FLAG_JumpPressed;
	if (Saved_bWantsToSprint) CompressedFlags |= FLAG_Custom_0;
	if (Saved_bWantsToSprintBoost) CompressedFlags |= FLAG_Custom_1;

	return CompressedFlags;

}

void UCSprintMovementComponent::FSavedMove_C::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{

	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UCSprintMovementComponent* MovementComponent = Cast<UCSprintMovementComponent>(Character->GetCharacterMovement());
	if (MovementComponent)
	{

		Saved_bCPressedJump = MovementComponent->CCharacterOwner->bCPressedJump;
		Saved_bWantsToSprint = MovementComponent->Safe_bWantsToSprint;
		Saved_bWantsToSprintBoost = MovementComponent->Safe_bWantsToSprintBoost;

	}

}

void UCSprintMovementComponent::FSavedMove_C::PrepMoveFor(ACharacter* Character)
{

	Super::PrepMoveFor(Character);

	UCSprintMovementComponent* MovementComponent = Cast<UCSprintMovementComponent>(Character->GetCharacterMovement());
	if (MovementComponent)
	{

		MovementComponent->CCharacterOwner->bCPressedJump = Saved_bCPressedJump;
		MovementComponent->Safe_bWantsToSprint = Saved_bWantsToSprint;
		MovementComponent->Safe_bWantsToSprintBoost = Saved_bWantsToSprintBoost;

	}

}

UCSprintMovementComponent::FNetworkPredictionData_Client_C::FNetworkPredictionData_Client_C(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{



}

FSavedMovePtr UCSprintMovementComponent::FNetworkPredictionData_Client_C::AllocateNewMove()
{

	return FSavedMovePtr(new FSavedMove_C());

}

FNetworkPredictionData_Client* UCSprintMovementComponent::GetPredictionData_Client() const
{

	check(PawnOwner != NULL);

	if (ClientPredictionData == nullptr)
	{

		UCSprintMovementComponent* MutableThis = const_cast<UCSprintMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_C(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;

	}

	return ClientPredictionData;

}

void UCSprintMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

float UCSprintMovementComponent::GetMaxSpeed() const
{

	if (MovementMode != MOVE_Custom) return Super::GetMaxSpeed();

	switch (CustomMovementMode)
	{

		case 'a': 

		default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"));
		return -1.f;

	}

}

float UCSprintMovementComponent::GetMaxBrakingDeceleration() const
{

	if (MovementMode != MOVE_Custom) return Super::GetMaxBrakingDeceleration();

	switch (CustomMovementMode)
	{

		case 'a': 

		default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"));
		return -1.f;

	}

}

bool UCSprintMovementComponent::CanAttemptJump() const
{

	return Super::CanAttemptJump();

}

bool UCSprintMovementComponent::DoJump(bool bReplayingMoves)
{

	if (Super::DoJump(bReplayingMoves))
	{

	}

	return false;

}

void UCSprintMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{

	//TryMantle
	if (CCharacterOwner->bCPressedJump)
	{


	}

	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);

}

void UCSprintMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{

	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

}

void UCSprintMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{

	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	Safe_bWantsToSprintBoost = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;

}

void UCSprintMovementComponent::OnClientCorrectionReceived(FNetworkPredictionData_Client_Character& ClientData, float TimeStamp, FVector NewLocation, FVector NewVelocity, UPrimitiveComponent* NewBase, FName NewBaseBoneName, bool bHasBase, bool bBaseRelativePosition, uint8 ServerMovementMode, FVector ServerGravityDirection)
{

	Super::OnClientCorrectionReceived(ClientData, TimeStamp, NewLocation, NewVelocity, NewBase, NewBaseBoneName, bHasBase, bBaseRelativePosition, ServerMovementMode, ServerGravityDirection);

	CorrectionCount++;

}

void UCSprintMovementComponent::InitializeComponent()
{

	Super::InitializeComponent();

	CCharacterOwner = Cast<APlayerCSprintCharacter>(GetOwner());

}

bool UCSprintMovementComponent::ServerCheckClientError(float ClientTimeStamp, float DeltaTime, const FVector& Accel, const FVector& ClientWorldLocation, const FVector& RelativeClientLocation, UPrimitiveComponent* ClientMovementBase, FName ClientBaseBoneName, uint8 ClientMovementMode)
{

	if (GetCurrentNetworkMoveData()->NetworkMoveType == FCharacterNetworkMoveData::ENetworkMoveType::NewMove)
	{

		float LocationError = FVector::Dist(UpdatedComponent->GetComponentLocation(), ClientWorldLocation);
		GEngine->AddOnScreenDebugMessage(6, 100.f, FColor::Yellow, FString::Printf(TEXT("Loc: %s"), *ClientWorldLocation.ToString()));
		AccumulatedClientLocationError += LocationError * DeltaTime;

	}

	return Super::ServerCheckClientError(ClientTimeStamp, DeltaTime, Accel, ClientWorldLocation, RelativeClientLocation, ClientMovementBase, ClientBaseBoneName, ClientMovementMode);

}

void UCSprintMovementComponent::CallServerMovePacked(const FSavedMove_Character* NewMove, const FSavedMove_Character* PendingMove, const FSavedMove_Character* OldMove)
{

	// Get storage container we'll be using and fill it with movement data
	FCharacterNetworkMoveDataContainer& MoveDataContainer = GetNetworkMoveDataContainer();
	MoveDataContainer.ClientFillNetworkMoveData(NewMove, PendingMove, OldMove);

	// Reset bit writer without affecting allocations
	FBitWriterMark BitWriterReset;
	BitWriterReset.Pop(CServerMoveBitWriter);

	// 'static' to avoid reallocation each invocation
	static FCharacterServerMovePackedBits PackedBits;
	UNetConnection* NetConnection = CharacterOwner->GetNetConnection();

	{

		// Extract the net package map used for serializing object references.
		CServerMoveBitWriter.PackageMap = NetConnection ? ToRawPtr(NetConnection->PackageMap) : nullptr;

	}

	if (CServerMoveBitWriter.PackageMap == nullptr)
	{

		UE_LOG(LogNetPlayerMovement, Error, TEXT("CallServerMovePacked: Failed to find a NetConnection/PackageMap for data serialization!"));
		return;

	}

	// Serialize move struct into a bit stream
	if (!MoveDataContainer.Serialize(*this, CServerMoveBitWriter, CServerMoveBitWriter.PackageMap) || CServerMoveBitWriter.IsError())
	{

		UE_LOG(LogNetPlayerMovement, Error, TEXT("CallServerMovePacked: Failed to serialize out movement data!"));
		return;

	}

	// Copy bits to our struct that we can NetSerialize to the server.
	PackedBits.DataBits.SetNumUninitialized(CServerMoveBitWriter.GetNumBits());

	check(PackedBits.DataBits.Num() >= CServerMoveBitWriter.GetNumBits());
	FMemory::Memcpy(PackedBits.DataBits.GetData(), CServerMoveBitWriter.GetData(), CServerMoveBitWriter.GetNumBytes());

	TotalBitsSent += PackedBits.DataBits.Num();

	// Send bits to server!
	ServerMovePacked_ClientSend(PackedBits);

	MarkForClientCameraUpdate();

}

void UCSprintMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{

	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{

		case 'a': 

		default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"));

	}

}

void UCSprintMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{

	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

}

void UCSprintMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{

	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{

		if (Safe_bWantsToSprintBoost == false && Safe_bWantsToSprint == true)
		{

			MaxWalkSpeed = Sprint_MaxWalkSpeed;

		}

		if (Safe_bWantsToSprintBoost == false && Safe_bWantsToSprint == false)
		{

			MaxWalkSpeed = Walk_MaxWalkSpeed;

		}

		if (Safe_bWantsToSprintBoost == true && Safe_bWantsToSprint == true)
		{

			MaxWalkSpeed = Sprint_Boost;

		}

	}

}

#pragma endregion

#pragma region SprintFunction

void UCSprintMovementComponent::SprintPressed()
{

	Safe_bWantsToSprint = true;

}

void UCSprintMovementComponent::SprintReleased()
{

	Safe_bWantsToSprint = false;

}

void UCSprintMovementComponent::SprintBoost()
{

	UE_LOG(LogTemp, Warning, TEXT("Boosting Execution: Remaining: %f!"), Sprint_BoostCount);

	if (Sprint_BoostCount > 0) {

		UE_LOG(LogTemp, Warning, TEXT("Boosting Execution!"));

		Safe_bWantsToSprintBoost = true;

		Sprint_BoostCount--;

	}

	BoostTimer();

}

void UCSprintMovementComponent::ResetSafeWantsToSprintBoost()
{

	Safe_bWantsToSprintBoost = false;

}

void UCSprintMovementComponent::BoostTimer()
{

	FTimerHandle SprintBoostResetTimer;

	GetWorld()->GetTimerManager().ClearTimer(SprintBoostResetTimer);

	GetWorld()->GetTimerManager().SetTimer(SprintBoostResetTimer, this, &UCSprintMovementComponent::ResetSafeWantsToSprintBoost, Sprint_BoostTimer, true);

}

#pragma endregion
