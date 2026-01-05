// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeHelper.h"

#include "AbilitySystemBlueprintLibrary.h"

DEFINE_LOG_CATEGORY(LogAttributeHelper);

const UAttributeSet* UAttributeHelper::GetAttributeSet(AActor* TargetActor, TSubclassOf<UAttributeSet> AttributeSetClass)
{
	const auto AbilityComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!AbilityComp) return nullptr;

	return AbilityComp->GetAttributeSet(AttributeSetClass);
}
