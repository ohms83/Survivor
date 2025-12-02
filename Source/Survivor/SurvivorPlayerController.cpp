// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivorPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Survivor.h"
#include "Components/Battle/AttackComponent.h"

ASurvivorPlayerController::ASurvivorPlayerController()
{
	bIsTouch = false;
	bMoveToMouseCursor = false;

	// configure the controller
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ASurvivorPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Only set up input on local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			// Setup click/touch input events
			EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Started, this, &ASurvivorPlayerController::HandleSetDestinationStarted);
			EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Triggered, this, &ASurvivorPlayerController::HandleSetDestinationTriggered);
			EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Completed, this, &ASurvivorPlayerController::HandleSetDestinationReleased);
			EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Canceled, this, &ASurvivorPlayerController::HandleSetDestinationReleased);

			// Setup attack input events
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASurvivorPlayerController::HandleAttackStarted);
		}
		else
		{
			UE_LOG(LogSurvivor, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
		}
	}
}

void ASurvivorPlayerController::HandleAttackStarted()
{
	const auto PlayerController = Cast<APawn>(GetPawn());
	if (!PlayerController) return;

	auto AttackComponent = PlayerController->GetComponentByClass<UAttackComponent>();
	if (!AttackComponent) return;

	AttackComponent->PerformAttack();
}

void ASurvivorPlayerController::HandleSetDestinationStarted(const FInputActionValue& InputValue)
{
	if (InputValue.GetMagnitude() > 1.0f) bIsTouch = true;
	StopMovement();
}

void ASurvivorPlayerController::HandleSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	if (APawn* ControlledPawn = GetPawn(); ControlledPawn != nullptr)
	{
		const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ASurvivorPlayerController::HandleSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			FXCursor,
			CachedDestination,
			FRotator::ZeroRotator,
			FVector(1.f, 1.f, 1.f),
			true, true,
			ENCPoolMethod::None,
			true);
	}

	FollowTime = 0.f;
	bIsTouch = false;
}
