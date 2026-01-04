// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageComponent.generated.h"

class UGameplayEffect;

DECLARE_LOG_CATEGORY_EXTERN(LogDamageComponent, Log, All);

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Instigator = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Receiver = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0;
	// TODO: More useful information, eg., critical flag.
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateOnDamageRecieved, const FDamageInfo&, DamageInfo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVOR_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageComponent();

	/**
	* Damage the owner with the specified damage.
	*/
	UFUNCTION(BlueprintCallable, Category="Damage")
	void Damage(float Damage, AActor* Instigator = nullptr) const;

	UFUNCTION(BlueprintCallable, Category="Damage")
	void KnockBack(float Magnitude, AActor* Instigator = nullptr) const;

	UPROPERTY(BlueprintAssignable, Category="Damage")
	FDelegateOnDamageRecieved EventDamageReceived;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	* Modify the target attribute of the component's owner via DamageEffect. 
	*/
	void ApplyDamageEffect(const FName& TargetAttribute, float Magnitude, float SkillLevel, AActor* Instigator) const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// TODO: Move to the FAttackData
	UPROPERTY(EditDefaultsOnly, Category="Damage", meta=(AllowPrivateAccess=true))
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	// TODO: Move to the FAttackData
	UPROPERTY(EditDefaultsOnly, Category="Damage", meta=(AllowPrivateAccess=true))
	TSubclassOf<UGameplayEffect> KnockBackEffectClass;
};
