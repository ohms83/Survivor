// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Battle/DamageComponent.h"

#include "AbilitySystemComponent.h"

DEFINE_LOG_CATEGORY(LogDamageComponent);

// Sets default values for this component's properties
UDamageComponent::UDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDamageComponent::ApplyDamageEffect(const FName& TargetAttribute, const float Magnitude, float SkillLevel, AActor* Instigator) const
{
// 	if (!IsValid(DamageEffectClass))
// 	{
// 		UE_LOG(LogDamageComponent, Error, TEXT("DamageEffect wasn't initialized!!"));
// 		return;
// 	}
//
// 	const auto Owner = GetOwner();
// 	auto AbilityComp = Owner->GetComponentByClass<UAbilitySystemComponent>();
// 	if (!AbilityComp)
// 	{
// 		UE_LOG(LogDamageComponent, Error, TEXT("Owner does not have an AbilitySystemComponent."));
// 		return;
// 	}
//
// 	FGameplayEffectContextHandle EffectContext = AbilityComp->MakeEffectContext();
// 	// Effect causer refers to the weapon that cause this damage, but it also can be the same as instigator.
// 	EffectContext.AddInstigator(Instigator, Instigator); 
//
// 	// Create the Effect Spec Handle 
// 	const FGameplayEffectSpecHandle SpecHandle = AbilityComp->MakeOutgoingSpec(
// 		DamageEffectClass.Get(),	// The UGameplayEffect class (Blueprint Asset)
// 		SkillLevel,
// 		EffectContext
// 	);
//
// 	const auto EffectSpecs = SpecHandle.Data;
// 	if (!EffectSpecs.IsValid())
// 	{
// 		UE_LOG(LogDamageComponent, Error, TEXT("Failed to create Gameplay Effect Spec."));
// 		return;
// 	}
//
// 	// Modify the Magnitude: Set the Damage value as a magnitude override.
// 	EffectSpecs->SetSetByCallerMagnitude(
// 		FGameplayTag::RequestGameplayTag(TargetAttribute),
// 		Magnitude
// 	);
//
// 	AbilityComp->ApplyGameplayEffectSpecToSelf(*EffectSpecs);
// #if 1
// 	UE_LOG(LogDamageComponent, Display, TEXT("Applied Damage Effect to %s. Attributes=%s Magnitude=%.3f Instigator=%s"),
// 		GetOwner() ? *GetOwner()->GetName() : TEXT("NULL"),
// 		*TargetAttribute.ToString(),
// 		Magnitude,
// 		Instigator ? *Instigator->GetName() : TEXT("NULL"));
// #endif
}

void UDamageComponent::Damage(float Damage, AActor* Instigator) const
{
	ApplyDamageEffect(
		FName("Battle.Damage.Value"),
		-1.0f * Damage,
		1.0f,	// TODO: Skill level
		Instigator);

	EventDamageReceived.Broadcast({
		Instigator,
		GetOwner(),
		Damage
	});
}

void UDamageComponent::KnockBack(const float Magnitude, AActor* Instigator) const
{
	ApplyDamageEffect(
		FName("Battle.Damage.KnockBack"),
		Magnitude,
		1.0f,	// TODO: Skill level
		Instigator);
}
