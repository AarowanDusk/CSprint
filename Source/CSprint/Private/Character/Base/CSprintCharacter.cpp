// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base/CSprintCharacter.h"
#include "Component/CustomMovementComponent/CSprintMovementComponent.h"

// Sets default values
ACSprintCharacter::ACSprintCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCSprintMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACSprintCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

