// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AttributeHelper.generated.h"

class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeChangedDelegate, float, NewValue, float, OldValue);

/**
 * 
 */
UCLASS()
class SURVIVOR_API UAttributeHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Get an attribute set of the specified type from the target actor.
	 * @param TargetActor 
	 * @param AttributeSetClass 
	 * @return A pointer to the requested attribute set or nullptr if not found. 
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeHeler")
	static const UAttributeSet* GetAttributeSet(const AActor* TargetActor, TSubclassOf<UAttributeSet> AttributeSetClass);

	/**
	 * Get an attribute set of the specified type from the target actor.
	 * @tparam AttributeSetType 
	 * @param TargetActor 
	 * @return A pointer to the requested attribute set or nullptr if not found. 
	 */
	template<typename AttributeSetType>
	static const AttributeSetType* GetAttributeSet(const AActor* TargetActor)
	{
		const auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetActor);
		if (!AbilitySystemInterface) return nullptr;

		const auto GasComp = AbilitySystemInterface->GetAbilitySystemComponent();
		if (!GasComp) return nullptr;

		return GasComp->GetSet<AttributeSetType>();
	}
};
