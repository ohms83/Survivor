// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Battle/TargetComponent.h"
#include "Components/SphereComponent.h"

#if !UE_BUILD_SHIPPING
#include "DrawDebugHelpers.h"
#endif

DEFINE_LOG_CATEGORY(LogTargetComponent);

// Sets default values for this component's properties
UTargetComponent::UTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	auto Owner = GetOwner();
	if (!IsValid(Owner)) return; // Just skip.

	auto SphereComponent = NewObject<USphereComponent>(Owner, TEXT("Target Trigger"));
	check(SphereComponent);

	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTargetEntered);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnTargetExited);
	TargetRadius = SphereComponent;
}

// Called when the game starts
void UTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (TargetRadius) TargetRadius->SetSphereRadius(TargetRange);
}

void UTargetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UTargetComponent::OnRegister()
{
	Super::OnRegister();
	
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return;

	if (TargetRadius)
	{
		// Scene component must be attached to the actor's root component
		TargetRadius->AttachToComponent(
			OwnerActor->GetRootComponent(),
			FAttachmentTransformRules::KeepRelativeTransform
		);
		TargetRadius->SetMobility(EComponentMobility::Movable);
		TargetRadius->SetSphereRadius(TargetRange);

		// Call RegisterComponent() *only* if it hasn't been done elsewhere
		// (If created in constructor, NewObject may have done initial setup)
		if (!TargetRadius->IsRegistered()) TargetRadius->RegisterComponent();
	}
}

// Called every frame
void UTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
#if !UE_BUILD_SHIPPING
	if (bDrawTargetRange && GetWorld() && TargetRadius)
	{
		DrawDebugSphere(
			GetWorld(),
			TargetRadius->GetComponentLocation(),	// Center of the sphere
			TargetRadius->GetScaledSphereRadius(),	// Radius of the sphere
			16,                                     // Number of segments
			FColor::Orange,                         // Color
			false                                   // Persistence (false for single frame, true for duration)
		);
	}
#endif
}

AActor* UTargetComponent::SelectTarget(ETargetSelectType SelectType)
{
	if (TargetActors.IsEmpty()) return nullptr;

	switch (SelectType)
	{
	case ETargetSelectType::First:
		return TargetActors[0];
	case ETargetSelectType::Closet:
		return FindClosetTarget(TargetActors);
	case ETargetSelectType::Random:
		return TargetActors.Num() == 1 ? TargetActors[0] : TargetActors[FMath::RandRange(0, TargetActors.Num() - 1)];
	default:
		ensureMsgf(false, TEXT("Invalid TargetSelectType"));
		return nullptr;
	}
}

bool UTargetComponent::SelectMultiTarget(ETargetSelectType SelectType, int32 NumTarget, TArray<AActor*>& OutTargets)
{
	// TODO:
	return false;
}

void UTargetComponent::OnTargetEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTargetComponent, Display, TEXT("Overlapping OtherActor=%s"), *OtherActor->GetName());
	if (ContainsTag(OtherActor, EnemyTags))
	{
		TargetActors.AddUnique(OtherActor);
		EventTargetAdded.Broadcast(OtherActor);
		UE_LOG(LogTargetComponent, Display, TEXT("Target Added. Target=%s"), *OtherActor->GetName());
	}
}

void UTargetComponent::OnTargetExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTargetComponent, Display, TEXT("End overlapping OtherActor=%s"), *OtherActor->GetName());
	if (ContainsTag(OtherActor, EnemyTags))
	{
		TargetActors.Remove(OtherActor);
		EventTargetRemoved.Broadcast(OtherActor);
		UE_LOG(LogTargetComponent, Display, TEXT("Target Removed. Target=%s"), *OtherActor->GetName());
	}
}

AActor* UTargetComponent::FindClosetTarget(const TArray<AActor*>& TargetList) const
{
	float ShortestDistance = FLT_MAX;
	AActor* ClosestTarget = nullptr;
	const auto& OwnerLocation = GetOwner()->GetActorLocation();

	for (const auto Actor : TargetList)
	{
		if (const auto Distance = FVector::Distance(OwnerLocation, Actor->GetActorLocation());  Distance < ShortestDistance)
		{
			ShortestDistance = Distance;
			ClosestTarget = Actor;
		}
	}

	return ClosestTarget;
}

bool UTargetComponent::ContainsTag(AActor* Actor, const TArray<FName>& TagList)
{
	for (const auto Tag : Actor->Tags)
	{
		if (TagList.Contains(Tag)) return true;
	}
	return false;
}

