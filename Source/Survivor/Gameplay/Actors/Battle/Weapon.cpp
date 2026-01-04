// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Actors/Battle/Weapon.h"

#include "SurvivorCharacter.h"
#include "Components/ShapeComponent.h"
#include "Components/Battle/DamageComponent.h"
#include "Components/Battle/TargetComponent.h"

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
	if (const auto TargetComponent = OtherActor->GetComponentByClass<UTargetComponent>(); !TargetComponent->CanBeTarget(OwnerCharacter)) return;
	// Check whether colliding with a hurt-box
	if (!OtherComp->ComponentHasTag(HurtBoxName)) return;

	auto DamageComponent = OtherActor->GetComponentByClass<UDamageComponent>();
	if (!IsValid(DamageComponent)) return;

	// TODO: Use gameplay effect.
	// OwnerBaseAttributes = IAttributeHolder::Execute_GetBaseAttributes(OwnerCharacter);
	// const auto OtherBaseAttributes = IAttributeHolder::Execute_GetBaseAttributes(OtherActor);
	// const auto Damage = UDamageCalculator::CalculateDamage(OwnerBaseAttributes, OtherBaseAttributes);
	// DamageComponent->Damage(Damage, OwnerCharacter);

	// TODO: Attack attributes
	// DamageComponent->KnockBack(500.f, OwnerCharacter);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

