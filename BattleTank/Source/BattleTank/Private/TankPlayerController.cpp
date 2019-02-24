// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"



void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("Not possessing a tank"));
	}
	else {
		FString TankName = ControlledTank->GetName();
		UE_LOG(LogTemp, Warning, TEXT("TankName: %s"), *TankName);
	}
	
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardCrosshair();
}

void ATankPlayerController::AimTowardCrosshair()
{
	if (!GetControlledTank()) {
		return;
	}

	FVector HitLocation; //Out Parameter

	if (GetSightRayHitLocation(HitLocation)) {

		
		UE_LOG(LogTemp, Warning, TEXT("HitLocation in player controller: %s"), *HitLocation.ToString());
		GetControlledTank()->AimAt(HitLocation);

	}
}



ATank* ATankPlayerController::GetControlledTank() const {

	return Cast<ATank>(GetPawn());
}

bool ATankPlayerController::GetSightRayHitLocation (FVector &HitLocation) const {

	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);

	UE_LOG(LogTemp, Warning, TEXT("ScreenLocation: %s"), *ScreenLocation.ToString());

	FVector LookDirection;

	if(GetLookDirection(ScreenLocation, LookDirection)){
		UE_LOG(LogTemp, Warning, TEXT("Look Direction: %s"), *LookDirection.ToString());
		
		if (GetLookVectorHitLocation(LookDirection, HitLocation)) {
			UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitLocation.ToString());
			return true;
		};
		

	}
	return false;
	
	/*
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace
	FHitResult HitResult;
	auto LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * 1000;

	GetWorld()->LineTraceSingleByObjectType(HitResult, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_MAX), TraceParameters);

	HitLocation = HitResult.GetActor()->GetActorLocation();
	*/
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {
	FVector WorldLocation;

	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * LineTraceRange;
	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility)) {
		HitLocation = HitResult.Location;
		return true;
	}
	return false;
}

