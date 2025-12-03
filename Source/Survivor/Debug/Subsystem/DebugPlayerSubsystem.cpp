// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Subsystem/DebugPlayerSubsystem.h"

#include "SurvivorCharacter.h"
#include "HAL/IConsoleManager.h"
#include "Gameplay/Character/Attribute/BaseAttributes.h"

DEFINE_LOG_CATEGORY(DebugPlayerSubsystem);

#define DECLARE_DEBUG_COMMAND_ARG(CommandName, HelpMsg) \
	DebugCommands.Add(IConsoleManager::Get().RegisterConsoleCommand( \
		*FString::Printf(TEXT("Survivor.DebugPlayerSubsytem.%s"), TEXT(#CommandName)), \
		TEXT(HelpMsg), \
		FConsoleCommandWithWorldAndArgsDelegate::CreateUObject(this, &ThisClass::CommandName), \
		ECVF_Default))

static bool GEnableDebugPlayerSubsystem = true;
static FAutoConsoleVariableRef CVarEnableDebugPlayerSubsystem (
	TEXT("Survivor.DebugPlayerSubsytem.Enable"),
	GEnableDebugPlayerSubsystem,
	TEXT("Enable/disable DebugPlayerSubsystem")
);

void UDebugPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LocalPlayer = GetLocalPlayer();

#if !UE_BUILD_SHIPPING
	DECLARE_DEBUG_COMMAND_ARG(PrintPlayerAttributes, "Print out the specified attribute's values.");
#endif
}

void UDebugPlayerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UDebugPlayerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
#if UE_BUILD_SHIPPING
	return false;
#else
	return GEnableDebugPlayerSubsystem;
#endif
}

void UDebugPlayerSubsystem::PrintPlayerAttributes(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 1)
	{
		UE_LOG(DebugPlayerSubsystem, Display, TEXT("Invalid argument list!"));
		return;
	}

	const auto ControlledChar = Cast<ASurvivorCharacter>(
		LocalPlayer->GetPlayerController(World)->GetCharacter());
	check(ControlledChar);
	
	const auto BaseAttributes = ControlledChar->GetBaseAttributes();
	for (const FString& Arg : Args)
	{
		bool bAttributeFound = false;
		float Value = 0;

		if (BaseAttributes)
		{
			if (Arg.Compare(FString(TEXT("Health")), ESearchCase::IgnoreCase) == 0)
			{
				bAttributeFound = true;
				Value = BaseAttributes->GetHealth();
			}
			else if (Arg.Compare(FString(TEXT("MaxHealth")), ESearchCase::IgnoreCase) == 0)
			{
				bAttributeFound = true;
				Value = BaseAttributes->GetMaxHealth();
			}
		}

		if (bAttributeFound)
		{
			UE_LOG(LogSurvivorCharacter, Log, TEXT("%s=%.3f"), *Arg, Value);
		}
		else
		{
			UE_LOG(LogSurvivorCharacter, Warning, TEXT("Cannot find an attribute named '%s'"), *Arg);
		}
	}
}
