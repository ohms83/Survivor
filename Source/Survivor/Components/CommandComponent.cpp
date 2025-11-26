// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CommandComponent.h"

DEFINE_LOG_CATEGORY(LogCommand);

// Sets default values for this component's properties
UCommandComponent::UCommandComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCommandComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCommandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	AActor* Executor = GetOwner();
	while (!Commands.IsEmpty())
	{
		UCommand* Command = nullptr;
		if (!Commands.Dequeue(Command)) break;

		Command->Execute(Executor);
	}
}

UCommand* UCommandComponent::NewCommand(const TSubclassOf<UCommand> CommandClass)
{
	if (!CommandClass.Get())
	{
	    UE_LOG(LogCommand, Error, TEXT("Invalid CommandClass!"));
	    return nullptr;
	}

	UCommand* NewCommand = NewObject<UCommand>(GetTransientPackage(), CommandClass.Get());
	Commands.Enqueue(NewCommand);
	return NewCommand;
}

void UCommand::Execute(AActor* ExecuteActor)
{
	if (IsValid(ExecuteActor)) OnExecuteCommand(ExecuteActor);
}

void UCommand::OnExecuteCommand_Implementation(AActor* ExecuteActor)
{
}
