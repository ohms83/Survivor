// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInteractable, Log, All);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SURVIVOR_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(Category="Interactable"))
	void Interact();
	/**
	 * Set whether this actor is interact-able. The Interact function should do nothing
     * if this is set to @c false. 
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(Category="Interactable"))
	void SetIsInteractable(bool bEnable);
	/**
	 * Determine whether this interactable can interact to the defined input action.
	 * @return true if the object can be interacted; otherwise, false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(Category="Interactable"))
	bool IsInteractable();
	/**
	 * Get a name of the specified input action this interface should be reacted to.
	 * @return An input action name. 
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(Category="Interactable"))
	FName GetInteractionInputAction();
};
