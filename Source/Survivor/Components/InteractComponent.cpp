// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractComponent.h"

#include "Components/SphereComponent.h"
#include "Gameplay/Interactable.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	InteractionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Sphere"));
	InteractionTrigger->SetSphereRadius(100.f);
	InteractionTrigger->CanCharacterStepUpOn = ECB_No;
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::OnComponentBeginOverlapped);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::OnComponentEndOverlapped);
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractComponent::Interact() const
{
	if (InteractingObject)
	{
		IInteractable::Execute_Interact(InteractingObject);
	}
}

void UInteractComponent::OnComponentEndOverlapped_Implementation(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	InteractingObject = nullptr;
}

void UInteractComponent::OnComponentBeginOverlapped_Implementation(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		InteractingObject = OtherActor;
	}
}
