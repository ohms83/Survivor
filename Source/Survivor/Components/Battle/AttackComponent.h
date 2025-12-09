// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

class UTargetComponent;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogAttackComponent, Log, All);

USTRUCT(Blueprintable, BlueprintType)
struct FAttackAnimData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage = nullptr;
	UPROPERTY(EditAnywhere)
	float AnimSpeed = 1.0f;	
};

UCLASS(BlueprintType)
class UAttackDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FAttackAnimData> AttackAnimations;
};

UCLASS( ClassGroup=("Gameplay/Battle"), meta=(BlueprintSpawnableComponent) )
class SURVIVOR_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttackComponent();

	UFUNCTION(BlueprintCallable, Category=Attack)
	void PerformAttack();
	/**
	 * Execute a saved combo attack.
	 * This is called from the anim blueprint.
	 */
	UFUNCTION(BlueprintCallable, Category=Attack)
	void ExecuteSavedAttack();
	UFUNCTION(BlueprintCallable, Category=Attack)
	void ResetCombo();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	TSoftObjectPtr<UAttackDataAsset> AttackDataAsset;
	/**
	 * If set, the component will automatically attack any nearby targets.
	 * @note TargetComponent is required.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	bool bAutoAttack = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * Play an attack montage from the ComboAttack list.
	 * @param Index Animation montage index. 
	 */
	void PlayAttackMontage(int32 Index);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	bool bIsAttacking = false;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	bool bSavedAttack = false;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	int32 ComboCount = 0;

	UPROPERTY()
	ACharacter* OwnerCharacter = nullptr;
	UPROPERTY()
	UTargetComponent* TargetComponent = nullptr;
};
