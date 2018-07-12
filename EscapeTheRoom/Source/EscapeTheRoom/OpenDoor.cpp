// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"


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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();
	// ...
	
}

void UOpenDoor::OpenDoor()
{
	if (!isOpen)
	{
		Owner->SetActorRotation(FRotator(0, OpenAngle, 0));
		isOpen = true;
		LastDoorOpenTime = GetWorld()->TimeSeconds;
	}
}

void UOpenDoor::CloseDoor()
{
	if (isOpen)
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
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) OpenDoor();

	if (isOpen && GetWorld()->TimeSeconds >= LastDoorOpenTime + DoorCloseDelay) CloseDoor();
	
}
