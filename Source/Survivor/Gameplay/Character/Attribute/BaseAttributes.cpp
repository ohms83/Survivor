// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Character/Attribute/BaseAttributes.h"
#include "GameplayEffectExtension.h"

void UBaseAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);// Check if the modified attribute was Health

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp Health between 0 and MaxHealth
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
}
