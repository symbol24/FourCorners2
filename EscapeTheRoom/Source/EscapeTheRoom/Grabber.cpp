// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsComponent();

	SetupInputComponent();

}

void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) UE_LOG(LogTemp, Error, TEXT("Physics handler NOT found on %s!"), *GetOwner()->GetName())
}

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

	// if the physics handled is attached
		// move the object held
	if (PhysicsHandle->GrabbedComponent)
	{
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
		//setup query parameters

		PhysicsHandle->SetTargetLocation(PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach);
	
	}
}

void UGrabber::Grab()
{
	auto ComponentToGrab = GetFirstPhysicsInReach();

	if(ComponentToGrab.GetActor()) PhysicsHandle->GrabComponentAtLocation(ComponentToGrab.GetComponent(), NAME_None, ComponentToGrab.GetActor()->GetActorLocation());
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsInReach()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	//setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	// ray cast out to reach distance or line trace
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewPointLocation,
		PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	if (Hit.GetActor())
	{
		ToOutput = "I hit:" + Hit.GetActor()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ToOutput);
	}

	return Hit;
}

