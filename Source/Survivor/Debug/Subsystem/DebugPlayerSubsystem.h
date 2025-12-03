// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "DebugPlayerSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(DebugPlayerSubsystem, Log, All);

struct IConsoleCommand;

/**
 * 
 */
UCLASS()
class SURVIVOR_API UDebugPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void PrintPlayerAttributes(const TArray<FString>& Args, UWorld* World);

private:
	UPROPERTY()
	ULocalPlayer* LocalPlayer;

	TArray<IConsoleCommand*> DebugCommands;
};
