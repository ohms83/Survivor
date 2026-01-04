// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeHelper.h"

const UAttributeSet* UAttributeHelper::GetAttributeSet(const AActor* TargetActor, TSubclassOf<UAttributeSet> AttributeSetClass)
{
	const auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetActor);
	if (!AbilitySystemInterface) return nullptr;

	const auto GasComp = AbilitySystemInterface->GetAbilitySystemComponent();
	if (!GasComp) return nullptr;

	return GasComp->GetAttributeSet(AttributeSetClass);
}
