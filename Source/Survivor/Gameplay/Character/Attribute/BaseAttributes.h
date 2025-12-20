// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBaseGameplayAttribute, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBaseAttributeChangedDelegate, float, NewValue, float, OldValue);

/**
 * Character's base attributes.
 */
UCLASS()
class SURVIVOR_API UBaseAttributes : public UAttributeSet
{
	GENERATED_BODY()
public:
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributes, Health);
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributes, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributes, Attack);
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributes, Defence);

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;;

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FBaseAttributeChangedDelegate OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FBaseAttributeChangedDelegate OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FBaseAttributeChangedDelegate OnAttackChanged;
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FBaseAttributeChangedDelegate OnDefenceChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Attack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Defence;
};
