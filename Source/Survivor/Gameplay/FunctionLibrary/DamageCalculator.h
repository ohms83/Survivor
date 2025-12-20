// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DamageCalculator.generated.h"

class UBaseAttributes;
/**
 * 
 */
UCLASS()
class SURVIVOR_API UDamageCalculator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "DamageCalculator")
	static float CalculateDamage(const UBaseAttributes* AttackerAttributes, const UBaseAttributes* DefenderAttributes);
};
