// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CommandComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCommand, Log, All);

UCLASS( ClassGroup=(Command), meta=(BlueprintSpawnableComponent) )
class SURVIVOR_API UCommand : public UObject
{
	GENERATED_BODY()

public:
	void Execute(AActor* ExecuteActor);

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnExecuteCommand(AActor* ExecuteActor);
};

UCLASS( ClassGroup=(Command), meta=(BlueprintSpawnableComponent) )
class SURVIVOR_API UCommandComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCommandComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void QueueCommand(UCommand* NewCommand) { Commands.Enqueue(NewCommand); }

	/**
	 * Create a new command from the specified class and enqueue it to the command queue. 
	 * @param CommandClass The target command class.
	 * @return The created command.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCommand* NewCommand(TSubclassOf<UCommand> CommandClass);

private:
	TQueue<UCommand*> Commands;
};
