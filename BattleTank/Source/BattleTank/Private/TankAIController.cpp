// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankPlayerController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	ATank* PlayerTank = GetPlayerTank();
	if (!PlayerTank) {
		UE_LOG(LogTemp, Warning, TEXT("AI Tank can't find player"));
	}
	else {
		FString TankName = PlayerTank->GetName();
		UE_LOG(LogTemp, Warning, TEXT("AI Tank found player: %s"), *TankName);
	}

}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (GetPlayerTank()) {

		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
	}
	
}

ATank * ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) { return nullptr; }
	return Cast<ATank>(PlayerPawn);
}


ATank* ATankAIController::GetControlledTank() const {

	return Cast<ATank>(GetPawn());
}