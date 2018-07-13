// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	isOpen = false;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if (!Owner) UE_LOG(LogTemp, Error, TEXT("No Owner found on %s!"), *GetOwner()->GetName())

	// ...
	if(!PressurePlate) UE_LOG(LogTemp, Error, TEXT("No Pressure plate assigned on %s!"), *GetOwner()->GetName())
}

void UOpenDoor::OpenDoor()
{
	if (!isOpen && Owner)
	{
		Owner->SetActorRotation(FRotator(0, OpenAngle, 0));
		isOpen = true;
		LastDoorOpenTime = GetWorld()->TimeSeconds;
		OnOpenRequest.Broadcast();
	}
}

void UOpenDoor::CloseDoor()
{
	if (isOpen && Owner)
	{
		Owner->SetActorRotation(FRotator(0, ClosedAngle, 0));
		isOpen = false;
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// check to see if actor is in pressure plate to open door
	if (GetTotalMassOnPlate() >= MassForDoor)
	{
		OnOpenRequest.Broadcast();
		LastDoorOpenTime = GetWorld()->TimeSeconds;
	}
	else if (GetWorld()->TimeSeconds >= LastDoorOpenTime + DoorCloseDelay) OnCloseRequest.Broadcast();
	
}

float UOpenDoor::GetTotalMassOnPlate()
{
	float TotalMass = 0.f;

	if (PressurePlate)
	{
		// find all overlapping actors
		TArray<AActor*> OverlappingActors;
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		// iterate through them
		for (const auto* item : OverlappingActors)
		{
			TotalMass += item->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	return TotalMass;
}
