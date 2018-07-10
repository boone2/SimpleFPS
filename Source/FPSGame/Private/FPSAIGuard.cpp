// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "Engine/World.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
    : PawnSensingComp()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
    Super::BeginPlay();
    OriginalRotation = GetActorRotation();
    PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
    PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
}

void AFPSAIGuard::OnPawnSeen(APawn* Pawn)
{
    if (Pawn == nullptr)
        return;
    DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32, 12, FColor::Yellow, false, 10);
    AFPSGameMode* GameMode = GetWorld()->GetAuthGameMode<AFPSGameMode>();
    if (GameMode)
    {
        GameMode->CompleteMission(Pawn, false);
    }
}

void AFPSAIGuard::OnNoiseHeard(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
    DrawDebugSphere(GetWorld(), Location, 16, 12, FColor::Green, false, 10);
    FVector Direction = Location - GetActorLocation();
    Direction.Normalize();
    FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
    NewLookAt.Pitch = 0;
    SetActorRotation(NewLookAt);

    GetWorldTimerManager().ClearTimer(TimerHandle_ResetRotation);
    GetWorldTimerManager().SetTimer(TimerHandle_ResetRotation, this, &AFPSAIGuard::ResetRotation, 3);
}

void AFPSAIGuard::ResetRotation()
{
    SetActorRotation(OriginalRotation);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
