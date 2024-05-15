// Copyright, Bhargav Jt. Gogoi (Aarowan Vespera)


#include "Character/Player/PlayerCSprintCharacter.h"
#include "Camera/CameraComponent.h"
#include "Component/CustomMovementComponent/CSprintMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/CPlayerState.h"

APlayerCSprintCharacter::APlayerCSprintCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCSprintMovementComponent>(ACharacter::CharacterMovementComponentName))
{

    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    CSprintMovement = Cast<UCSprintMovementComponent>(GetCharacterMovement());

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

    // Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
    // instead of recompiling to adjust them
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
    // are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

    bReplicates = true;

}

void APlayerCSprintCharacter::PossessedBy(AController* NewController)
{

    Super::PossessedBy(NewController);

}

void APlayerCSprintCharacter::OnRep_PlayerState()
{

    Super::OnRep_PlayerState();

}

void APlayerCSprintCharacter::Jump()
{

    Super::Jump();

    bCPressedJump = true;

    bPressedJump = false;

}

void APlayerCSprintCharacter::StopJumping()
{

    Super::StopJumping();

    bCPressedJump = false;

}

FCollisionQueryParams APlayerCSprintCharacter::GetIgnoreCharacterParams() const
{

    FCollisionQueryParams Params;

    TArray<AActor*> CharacterChildren;
    GetAllChildActors(CharacterChildren);
    Params.AddIgnoredActors(CharacterChildren);
    Params.AddIgnoredActor(this);

    return Params;

}

void APlayerCSprintCharacter::Tick(float DeltaSeconds)
{

    Super::Tick(DeltaSeconds);

}

void APlayerCSprintCharacter::GetSprint()
{

    this->GetCharacterMovementComponent()->SprintPressed();

}

void APlayerCSprintCharacter::GetSprintRelease()
{

    this->GetCharacterMovementComponent()->SprintReleased();

}

void APlayerCSprintCharacter::GetBoost()
{

    this->GetCharacterMovementComponent()->SprintBoost();

}


