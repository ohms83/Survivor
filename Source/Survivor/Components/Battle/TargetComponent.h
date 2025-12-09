// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetComponent.generated.h"

class USphereComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTargetComponent, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetListUpdatedDelegate, AActor*, actor);

UENUM(BlueprintType)
enum class ETargetSelectType : uint8
{
	//! Select the first target in the list.
	First	UMETA(DisplayName = "First"),
	//! Select the closet target in the list
	Closet	UMETA(DisplayName = "Closet"),
	//! Select targets at random.
	Random	UMETA(DisplayName = "Random"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVOR_API UTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetComponent();

	//! Select a single target from the target list.
	UFUNCTION(BlueprintCallable, Category="Targetting")
	AActor* SelectTarget(ETargetSelectType SelectType);
	//! Select multiple targets from the list.
	UFUNCTION(BlueprintCallable, Category="Targetting")
	bool SelectMultiTarget(ETargetSelectType SelectType, int32 NumTarget, TArray<AActor*>& OutTargets);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnRegister() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Targetting")
	float TargetRange = 300.f;
	/**
	 * Enemy tags list. The component will only react to those actors with these tags.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Targetting")
	TArray<FName> EnemyTags;

	UPROPERTY(BlueprintAssignable, Category="Targetting")
	FTargetListUpdatedDelegate EventTargetAdded;
	UPROPERTY(BlueprintAssignable, Category="Targetting")
	FTargetListUpdatedDelegate EventTargetRemoved;

	/**
	 * DEBUG ONLY: If set, draws a visible sphere in the world representing the TargetRange.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targetting|Debug")
	bool bDrawTargetRange = false;

private:
	//! An overlap event triggered when any potential targets entering the target range.
	UFUNCTION()
	void OnTargetEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//! An overlap event triggered when any potential targets leaving the target range.
	UFUNCTION()
	void OnTargetExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	AActor* FindClosetTarget(const TArray<AActor*>& TargetList) const;
	//! Check whether the specified actor contains any of a tag in the tag list.  
	static bool ContainsTag(AActor* Actor, const TArray<FName>& TagList);

	UPROPERTY()
	TArray<AActor*> TargetActors;

	TObjectPtr<USphereComponent> TargetRadius;
};
