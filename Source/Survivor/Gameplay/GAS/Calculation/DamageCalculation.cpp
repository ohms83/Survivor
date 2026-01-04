// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GAS/Calculation/DamageCalculation.h"
#include "Gameplay/Character/Attribute/BaseAttributes.h"
#include "Gameplay/Character/Attribute/BattleAttributes.h"

struct FSurvivorDamageStatics
{
	// Define the captures for Source and Target attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageValue);

	FSurvivorDamageStatics()
	{
		// Capture Attack from the SOURCE (the attacker)
		// Set 'bSnapshot' to true if you want the value at the moment the effect is applied
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributes, Attack, Source, true);

		// Capture Armor and Health from the TARGET (the victim)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributes, Defence, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBattleAttributes, DamageValue, Target, true);
	}
};

static const FSurvivorDamageStatics& DamageStatics()
{
	static FSurvivorDamageStatics DStatics;
	return DStatics;
}

UDamageCalculation::UDamageCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenceDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageValueDef);
}

void UDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	// Get the source/target tags and the effect spec
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Capture the attribute values as floats
	float AttackPower = 0.f;
	float Armor = 0.f;
	// Get the actual value of attributes at this exact moment.
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDef, EvaluationParameters, AttackPower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenceDef, EvaluationParameters, Armor);

	// Damage computation
	const float Variant = FMath::RandRange(0.85f, 1.15f);
	const float MitigatedDamage = FMath::RoundToFloat(Variant * (FMath::Pow(AttackPower, 1.25f) / (1.0f + Armor)));

	// APPLY THE MODIFIER
	// Instead of subtracting health, we apply it to the "Damage" Meta-Attribute 
	// This allows the AttributeSet to handle the actual health subtraction and clamping.
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageValueProperty, EGameplayModOp::Override, MitigatedDamage));
}
