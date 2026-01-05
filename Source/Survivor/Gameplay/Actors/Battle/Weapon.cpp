// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Actors/Battle/Weapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "SurvivorCharacter.h"
#include "Components/ShapeComponent.h"
#include "Components/Battle/TargetComponent.h"
#include "Gameplay/GAS/Attribute/AttributeHelper.h"

static const FName HurtBoxName = TEXT("HurtBox");

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	OwnerCharacter = Cast<ASurvivorCharacter>(NewOwner);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HitBoxComponent))
	{
		HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginHit);
	}
}

void AWeapon::OnBeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OwnerCharacter)) return;
	// Check whether the other actor can be a target.
	if (const auto OtherTargetComponent = OtherActor->GetComponentByClass<UTargetComponent>(); !OtherTargetComponent->CanBeTarget(OwnerCharacter)) return;
	// Check whether colliding with a hurt-box
	if (!OtherComp->ComponentHasTag(HurtBoxName)) return;
	
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerCharacter);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

	if (!IsValid(OwnerASC) || !IsValid(TargetASC)) return;

	for (const auto& Effect : DamageEffects)
	{
		FGameplayEffectContextHandle ContextHandle = OwnerASC->MakeEffectContext();
		ContextHandle.AddInstigator(OwnerCharacter, OwnerCharacter);

		if (FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(Effect, 1.0f, ContextHandle); SpecHandle.IsValid())
		{
			// 3. Apply the Effect to the Target
			OwnerASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
		}	
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

