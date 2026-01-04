// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Battle/AttackComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "TargetComponent.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY(LogAttackComponent);

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	// The owner must be a character.
	check(OwnerCharacter);

	TargetComponent = Cast<UTargetComponent>(OwnerCharacter->FindComponentByClass<UTargetComponent>());
	if (!TargetComponent)
	{
		UE_LOG(LogAttackComponent, Warning, TEXT("Character %s doesn't have a Target Component!"),
			*OwnerCharacter->GetName());
	}

	(void)AttackDataAsset.LoadSynchronous();

	if (!IsValid(AttackAbility.Get()))
	{
		UE_LOG(LogAttackComponent, Error, TEXT("Invalid Attack Ability! Character=%s"),
			*OwnerCharacter->GetName());
		return;
	}

	if (const auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(OwnerCharacter))
	{
		OwnerGAS = AbilitySystemInterface->GetAbilitySystemComponent();
		if (OwnerGAS) {
			AttackAbilityHandle = OwnerGAS->GiveAbility(OwnerGAS->BuildAbilitySpecFromClass(AttackAbility.Get(), 0, -1));
		}
	}
	else
	{
		UE_LOG(LogAttackComponent, Error, TEXT("Actor (%s) doesn't implement IAbilitySystemInterface or never initialized UGameplayAbilitySystemComponent."),
			OwnerCharacter ? *OwnerCharacter->GetName() : TEXT("NULL")
		);
	}
}

void UAttackComponent::PlayAttackMontage(const int32 Index)
{
	if (!AttackDataAsset)
	{
		UE_LOG(LogAttackComponent, Error, TEXT("Invalid AttackDataAsset!"));
		return;
	}

	const auto& ComboAttacks = AttackDataAsset->AttackAnimations;
	if (ComboAttacks.IsEmpty() || !ComboAttacks.IsValidIndex(Index))
	{
		UE_LOG(LogAttackComponent, Warning, TEXT("Invalid anim montage index. Index=%d Num Montage=%d"),
			Index, ComboAttacks.Num());
		return;
	}
	const auto& AttackData = ComboAttacks[Index];
	check(AttackData.Montage);
	OwnerCharacter->PlayAnimMontage(AttackData.Montage, AttackData.AnimSpeed);
}

// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (!bAutoAttack || !IsValid(TargetComponent) || bSavedAttack) return;

	// TODO: Make target select type to be configurable.
	const auto Target = TargetComponent->SelectTarget(ETargetSelectType::First);
	if (!IsValid(Target)) return;

	PerformAttack();
}

void UAttackComponent::PerformAttack()
{
	// Another attack was already queued.
	if (bSavedAttack) return;

	if (bIsAttacking)
	{
		// Save the attack command to be executed when receiving the anim notify event.
		UE_LOG(LogAttackComponent, Log, TEXT("Attack input saved.")); 
		bSavedAttack = true;
		return;
	}
	
	if (!AttackDataAsset)
	{
		UE_LOG(LogAttackComponent, Error, TEXT("Invalid AttackDataAsset!"));
		return;
	}

	bIsAttacking = true;

	// PlayAttackMontage(ComboCount++);
	// if (AttackAbility)
	if (!OwnerGAS)
	{
		UE_LOG(LogAttackComponent, Warning, TEXT("Actor (%s) doesn't implement IAbilitySystemInterface or never initialized UGameplayAbilitySystemComponent."),
			OwnerCharacter ? *OwnerCharacter->GetName() : TEXT("NULL")
		);
		return;
	}
	OwnerGAS->TryActivateAbility(AttackAbilityHandle);

	if (!AttackDataAsset->AttackAnimations.IsValidIndex(ComboCount)) ComboCount = 0;
}

void UAttackComponent::ExecuteSavedAttack()
{
	bIsAttacking = false;
	if (!bSavedAttack) return;
	
	UE_LOG(LogAttackComponent, Log, TEXT("Process saved attack."));
	bSavedAttack = false;
	PerformAttack();
}

void UAttackComponent::ResetCombo()
{
	bSavedAttack = false;
	bIsAttacking = false;
	ComboCount = 0;
}
