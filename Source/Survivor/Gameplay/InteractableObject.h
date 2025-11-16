// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractableObject.generated.h"

class UInputAction;
class USphereComponent;
class UTextRenderComponent;

UCLASS()
class SURVIVOR_API AInteractableObject : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bCanInteract = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (Category="Interaction"))
	UTextRenderComponent* Instruction = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (Category="Interaction"))
	UUserWidget* InteractionWidget = nullptr;
	/**
	 * The name of the input action that this interactor should be listened to. 
	 */
	UPROPERTY(EditDefaultsOnly, meta = (Category="Interaction"))
	FName InteractionAction = "Interact";

private:
	virtual void Interact_Implementation() override;
	virtual void ShowInstruction_Implementation(bool bVisible) override;

	virtual void SetIsInteractable_Implementation(const bool bEnable) override
	{
		bCanInteract = bEnable;
	}
	
	virtual bool IsInteractable_Implementation() override
	{
		return bIsInteractable;
	}
	
	virtual FName GetInteractionInputAction_Implementation() override
	{
		return InteractionAction;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Category="Interaction", AllowPrivateAccess=true))
	bool bIsInteractable = true;
	/**
	 * The name of an input action this object should interact.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Category="Interaction", AllowPrivateAccess=true))
	FName InputAction;
};
