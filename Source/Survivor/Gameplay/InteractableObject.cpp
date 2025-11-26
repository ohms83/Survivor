// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/InteractableObject.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Instruction = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Instruction Text"));
	Instruction->SetHorizontalAlignment(EHTA_Center);
	Instruction->SetRelativeLocation(FVector::ZeroVector);
	Instruction->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	Instruction->SetVisibility(false);
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!IsActorInteractable(OtherActor)) return;

	Instruction->SetVisibility(true);
}

void AInteractableObject::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (!IsActorInteractable(OtherActor)) return;

	Instruction->SetVisibility(false);
}

void AInteractableObject::Interact_Implementation()
{
	if (!IsInteractable() || !InteractionWidget) return;

	InteractionWidget->AddToViewport();
}

void AInteractableObject::ShowInstruction(bool bVisible)
{
	if (IsInteractable() && IsValid(Instruction)) Instruction->SetVisibility(bVisible);
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AInteractableObject::IsActorInteractable(const AActor* OtherActor) const
{
	for (const auto& Tag : OtherActor->Tags)
	{
		if (InteractorTags.Contains(Tag)) return true;
	}
	return false;
}

