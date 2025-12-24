// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DamageAttributes.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVOR_API UDamageAttributes : public UAttributeSet
{
	GENERATED_BODY()
public:
	ATTRIBUTE_ACCESSORS_BASIC(UDamageAttributes, KnockBackMagnitude);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData KnockBackMagnitude;
};
