// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/FunctionLibrary/DamageCalculator.h"
#include "Gameplay/Character/Attribute/BaseAttributes.h"

float UDamageCalculator::CalculateDamage(const UBaseAttributes* AttackerAttributes, const UBaseAttributes* DefenderAttributes)
{
	if (!AttackerAttributes || !DefenderAttributes) return 0;

	const float Variant = FMath::RandRange(0.85f, 1.15f);
	const float Damage = Variant * (FMath::Pow(AttackerAttributes->GetAttack(), 1.125f) / (1.0f + AttackerAttributes->GetDefence()));
	return Damage;
}
