// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
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
	UE_LOG(LogTemp, Error, TEXT("Grabber report reporting for duty!"));


	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// get player view point this tick



	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	// ToOutput = "Location: " + PlayerViewPointLocation.ToString() + " // Rotation: " + PlayerViewPointRotation.ToString();
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *ToOutput);

	// draw a red line to visualize
	// DrawDebugLine(GetWorld(), PlayerViewPointLocation, PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach, FColor(255, 0, 0), false, 0, 0, 10);

	// ray cast out to reach distance

	// see what we hit
}

