// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class SURVIVOR_API ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = Input)
	void RegisterAttack();
	
	UFUNCTION(BlueprintNativeEvent, Category = Input)
	void OnSetDestinationTriggered();
	UFUNCTION(BlueprintNativeEvent, Category = Input)
	void OnSetDestinationCompleted(const FInputActionInstance& InputAction);
	UFUNCTION(BlueprintNativeEvent, Category = Input)
	void OnSetDestinationCanceled(const FInputActionInstance& InputAction);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	TArray<UInputMappingContext*> MappingContexts;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* SetDestinationClickAction;

protected:
	void SetDestination(const FVector& Destination);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float PressedThreshold = 0.3f;

	FVector CachedDestination;
};
