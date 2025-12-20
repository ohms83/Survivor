// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UBaseAttributes;
class UGameplayAbility;
class UTargetComponent;
class UShapeComponent;

UCLASS()
class SURVIVOR_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	virtual void SetOwner(AActor* NewOwner) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hit Box", meta = (AllowPrivateAccess = "true"))
	UShapeComponent* HitBoxComponent = nullptr;

	TObjectPtr<class ASurvivorCharacter> OwnerCharacter;
	TObjectPtr<const UBaseAttributes> OwnerBaseAttributes;
};
