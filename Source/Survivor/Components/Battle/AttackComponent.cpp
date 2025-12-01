// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Battle/AttackComponent.h"

#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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

	// ...
	const auto* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PlayerController) return;

	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!InputSubsystem) return;
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		// Bind the Aim Action to be triggered when pressed/released
		UE_LOG(LogAttackComponent, Log, TEXT("Binding attack inputs"));
		EnhancedInputComponent->BindAction(AttackAction.Get(), ETriggerEvent::Started, this, &UAttackComponent::HandleAttackInput);
	}
}

void UAttackComponent::PlayAttackMontage(const int32 Index)
{
	if (ComboAttacks.IsEmpty() || !ComboAttacks.IsValidIndex(Index))
	{
		UE_LOG(LogAttackComponent, Warning, TEXT("Invalid anim montage index. Index=%d Num Montage=%d"),
			Index, ComboAttacks.Num());
		return;
	}
	OwnerCharacter->PlayAnimMontage(ComboAttacks[Index].Get());
}

// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackComponent::HandleAttackInput(const FInputActionValue& Value)
{
	UE_LOG(LogAttackComponent, Log, TEXT("Handle attack input"));
	PerformAttack();
}

void UAttackComponent::PerformAttack()
{
	if (bIsAttacking)
	{
		// Save the attack command to be executed when receiving the anim notify event.
		UE_LOG(LogAttackComponent, Log, TEXT("Attack input saved.")); 
		bSavedAttack = true;
		return;
	}

	bIsAttacking = true;

	PlayAttackMontage(ComboCount++);
	if (!ComboAttacks.IsValidIndex(ComboCount)) ComboCount = 0;
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
