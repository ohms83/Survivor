// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BattleAttributes.generated.h"

/**
 * An attribute set relating to the battle. Most attributes are mainly temporary and immediately consumed once
 * set.
 */
UCLASS()
class SURVIVOR_API UBattleAttributes : public UAttributeSet
{
	GENERATED_BODY()
public:
	ATTRIBUTE_ACCESSORS_BASIC(UBattleAttributes, DamageValue);
	ATTRIBUTE_ACCESSORS_BASIC(UBattleAttributes, KnockBackMagnitude);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData DamageValue;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData KnockBackMagnitude;

private:
	void ExecuteDamageValueEffect(const FGameplayEffectModCallbackData& Data);
	void ExecuteKnockBackMagnitudeEffect(const FGameplayEffectModCallbackData& Data);
};
