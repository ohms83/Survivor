// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SurvivorCharacter.generated.h"

class UBaseAttributes;
class UCameraComponent;
class USpringArmComponent;
class UCommandComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogSurvivorCharacter, Display, All);

/**
 *  A controllable top-down perspective character
 */
UCLASS(abstract)
class ASurvivorCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	/** Constructor */
	ASurvivorCharacter();

	/** Initialization */
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Update */
	virtual void Tick(float DeltaSeconds) override;

	/** Returns the camera component **/
	FORCEINLINE UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	/** Returns the Camera Boom component **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/**
	 * Returns the Ability System component
	 */
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}
	
	UFUNCTION(BlueprintGetter)
	const UBaseAttributes* GetBaseAttributes() const { return BaseAttributes; };

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent = nullptr;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom = nullptr;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCommandComponent* CommandComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;

	/** Base character's attribute Set. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintGetter=GetBaseAttributes, BlueprintReadOnly, Category="Attribute", meta = (AllowPrivateAccess = "true"))
	const UBaseAttributes* BaseAttributes = nullptr;
};

