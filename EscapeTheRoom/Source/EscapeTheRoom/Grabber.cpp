// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"

#define OUT // to indicate that certain pointers will be modified by a function

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsComponent();
	SetupInputComponent();

}

// Get the Physics component on the actor.
void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) UE_LOG(LogTemp, Error, TEXT("Physics handler NOT found on %s!"), *GetOwner()->GetName())
}

// Setting up inputs for the actor.
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) UE_LOG(LogTemp, Error, TEXT("Input component NOT found on %s!"), *GetOwner()->GetName())
	else
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// if the physics handled is attached move the object held
	if (PhysicsHandle != nullptr && PhysicsHandle->GrabbedComponent) PhysicsHandle->SetTargetLocation(GetReachLineEnd());
}

// Tries to grab an object in a ray cast
void UGrabber::Grab()
{
	auto ComponentToGrab = GetFirstPhysicsInReach();
	if(ComponentToGrab.GetActor()) PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab.GetComponent(), NAME_None, ComponentToGrab.GetActor()->GetActorLocation(),ComponentToGrab.GetActor()->GetActorRotation());
}

// Releases the grabbed object
void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

// Looks at the objects within a raycast at "reach" distance
const FHitResult UGrabber::GetFirstPhysicsInReach()
{
	//setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	// ray cast out to reach distance or line trace
	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		PlayerViewPointLocation,
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

// Gets the vector of length reach
FVector UGrabber::GetReachLineEnd()
{
	// get the player's view
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
