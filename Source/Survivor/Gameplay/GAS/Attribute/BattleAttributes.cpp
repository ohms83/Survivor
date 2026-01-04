// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAttributes.h"

#include "BaseAttributes.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"

void UBattleAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageValueAttribute())
	{
		ExecuteDamageValueEffect(Data);
	}
	else if (Data.EvaluatedData.Attribute == GetKnockBackMagnitudeAttribute())
	{
		ExecuteKnockBackMagnitudeEffect(Data);
	}
}

void UBattleAttributes::ExecuteDamageValueEffect(const FGameplayEffectModCallbackData& Data)
{
	if (const float Magnitude = GetDamageValue(); Magnitude > 0.f)
	{
		// Use the ASC to modify the attribute properly
		Data.Target.ApplyModToAttribute(UBaseAttributes::GetHealthAttribute(), EGameplayModOp::Additive, -Magnitude);

		// Clear the attribute so it doesn't persist
		SetDamageValue(0.f);
	}
}

void UBattleAttributes::ExecuteKnockBackMagnitudeEffect(const FGameplayEffectModCallbackData& Data)
{
	if (const float Magnitude = GetKnockBackMagnitude(); Magnitude > 0.f)
	{
		AActor* TargetActor = Data.Target.GetAvatarActor();
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);

		if (const AActor* SourceActor = Data.EffectSpec.GetContext().GetInstigator(); TargetCharacter && SourceActor)
		{
			FVector KnockBackDir = TargetCharacter->GetActorLocation() - SourceActor->GetActorLocation();
			KnockBackDir.Z = 0.1f; // Give it a slight upward lift
			KnockBackDir.Normalize();

			TargetCharacter->LaunchCharacter(KnockBackDir * Magnitude, true, true);
		}

		// Clear the attribute so it doesn't persist
		SetKnockBackMagnitude(0.f);
	}
}
