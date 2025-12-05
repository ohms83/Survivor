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

void UDamageComponent::Damage(float Damage, AActor* Instigator)
{
	if (!IsValid(DamageEffect))
	{
		UE_LOG(LogDamageComponent, Error, TEXT("DamageEffect wasn't initialized!!"));
		return;
	}

	const auto Owner = GetOwner();
	auto AbilityComp = Owner->GetComponentByClass<UAbilitySystemComponent>();
	if (!AbilityComp) 
	{
		UE_LOG(LogDamageComponent, Error, TEXT("Owner does not have an AbilitySystemComponent."));
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilityComp->MakeEffectContext();
	// Effect causer refers to the weapon that cause this damage, but it also can be the same as instigator.
	EffectContext.AddInstigator(Instigator, Instigator); 
    
	// Create the Effect Spec Handle 
	FGameplayEffectSpecHandle SpecHandle = AbilityComp->MakeOutgoingSpec(
		DamageEffectClass.Get(),	// The UGameplayEffect class (Blueprint Asset)
		1.0f,						// TODO: skill level
		EffectContext
	);

	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogDamageComponent, Error, TEXT("Failed to create Gameplay Effect Spec."));
		return;
	}

	// Modify the Magnitude: Set the Damage value as a magnitude override.
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Battle.Damage.Value")),
		-1.0f * Damage
	);

	AbilityComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	EventDamageReceived.Broadcast({
		Instigator,
		Owner,
		Damage
	});
#if 0
	UE_LOG(LogDamageComponent, Display, TEXT("%s received %.3f damage from %s"),
		*Owner->GetName(), Damage, *Instigator->GetName());
#endif
}


// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (const auto EffectClass = DamageEffectClass.Get())
	{
		DamageEffect = NewObject<UGameplayEffect>(GetTransientPackage(), EffectClass);
	}
}


// Called every frame
void UDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

