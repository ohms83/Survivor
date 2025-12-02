// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "SurvivorPlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  Player controller for a top-down perspective game.
 *  Implements point and click based controls
 */
UCLASS(abstract)
class ASurvivorPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, Category="Input")
	float ShortPressThreshold = 0.3f;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, Category="Input")
	UNiagaraSystem* FXCursor = nullptr;

	/** MappingContext */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* DefaultMappingContext = nullptr;
	
	/** Click to move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SetDestinationAction = nullptr;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AttackAction = nullptr;

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	/** Set to true if we're using touch input */
	uint32 bIsTouch : 1;

	/** Saved location of the character movement destination */
	FVector CachedDestination;

	/** Time that the click input has been pressed */
	float FollowTime = 0.0f;

public:

	/** Constructor */
	ASurvivorPlayerController();

protected:

	/** Initialize input bindings */
	virtual void SetupInputComponent() override;
	
	/** Input handlers */
	void HandleAttackStarted();
	void HandleSetDestinationStarted(const FInputActionValue& InputValue);
	void HandleSetDestinationTriggered();
	void HandleSetDestinationReleased();
};


