// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

/**
 * Character's base attributes.
 */
UCLASS()
class SURVIVOR_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributeSet, Health);
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributeSet, MaxHealth);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
};
