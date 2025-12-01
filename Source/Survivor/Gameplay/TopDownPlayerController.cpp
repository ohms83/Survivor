// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TopDownPlayerController.h"

#include "EnhancedInputSubsystems.h"

void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (int32 Index = 0; Index < MappingContexts.Num(); ++Index)
		{
			const auto* Context = MappingContexts[Index];
			check(Context);
			InputSubsystem->AddMappingContext(Context, Index);
		}
	}
}

void ATopDownPlayerController::RegisterAttack()
{
}

void ATopDownPlayerController::OnSetDestinationCanceled_Implementation(const FInputActionInstance& InputAction)
{
}

void ATopDownPlayerController::OnSetDestinationCompleted_Implementation(const FInputActionInstance& InputAction)
{
}

void ATopDownPlayerController::SetDestination(const FVector& Destination)
{
	CachedDestination = Destination;
}

void ATopDownPlayerController::OnSetDestinationTriggered_Implementation()
{
}
