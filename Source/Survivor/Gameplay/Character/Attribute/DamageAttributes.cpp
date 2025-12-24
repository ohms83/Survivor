// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Character/Attribute/DamageAttributes.h"

#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"

void UDamageAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetKnockBackMagnitudeAttribute())
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
}
