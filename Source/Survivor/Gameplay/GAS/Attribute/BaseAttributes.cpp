// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributes.h"
#include "GameplayEffectExtension.h"

DEFINE_LOG_CATEGORY(LogBaseGameplayAttribute);

void UBaseAttributes::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		// Clamp Health between 0 and MaxHealth
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UBaseAttributes::PostAttributeChange(const FGameplayAttribute& Attribute, const float OldValue, const float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	UE_LOG(LogBaseGameplayAttribute, Log, TEXT("Attribute '%s' value changed. Old=%.3f New=%.3f"),
		*Attribute.GetName(), OldValue, NewValue);

	const auto AttributeName = Attribute.GetName();

	if (AttributeName == GetHealthAttribute().GetName()) OnHealthChanged.Broadcast(NewValue, OldValue);
	else if (AttributeName == GetMaxHealthAttribute().GetName()) OnMaxHealthChanged.Broadcast(NewValue, OldValue);
	else if (AttributeName == GetAttackAttribute().GetName()) OnAttackChanged.Broadcast(NewValue, OldValue);
	else if (AttributeName == GetDefenceAttribute().GetName()) OnDefenceChanged.Broadcast(NewValue, OldValue);
}
