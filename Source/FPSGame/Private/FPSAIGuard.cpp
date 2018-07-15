// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "Engine/World.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
    : PawnSensingComp()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
    GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
    Super::BeginPlay();
    OriginalRotation = GetActorRotation();
    PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
    PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

    if (bPatrol)
        MoveToNextPatrolPoint();
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
    SetGuardState(EAIState::Alerted);

    AController *Controller = GetController();
    if (Controller)
    {
        Controller->StopMovement();
    }
}

void AFPSAIGuard::OnNoiseHeard(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
    if (GuardState == EAIState::Alerted)
    {
        return;
    }

    DrawDebugSphere(GetWorld(), Location, 16, 12, FColor::Green, false, 10);
    FVector Direction = Location - GetActorLocation();
    Direction.Normalize();
    FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
    NewLookAt.Pitch = 0;
    SetActorRotation(NewLookAt);

    GetWorldTimerManager().ClearTimer(TimerHandle_ResetRotation);
    GetWorldTimerManager().SetTimer(TimerHandle_ResetRotation, this, &AFPSAIGuard::ResetRotation, 3);

    SetGuardState(EAIState::Suspicious);
    AController *Controller = GetController();
    if (Controller)
    {
        Controller->StopMovement();
    }
}

void AFPSAIGuard::ResetRotation()
{
    if (GuardState == EAIState::Alerted)
        return;

    SetActorRotation(OriginalRotation);
    SetGuardState(EAIState::Idle);

    if (bPatrol)
    {
        MoveToNextPatrolPoint();
    }
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
    if (NewState == GuardState)
    {
        return;
    }

    GuardState = NewState;
    OnStateChanged(NewState);
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
    if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
    {
        CurrentPatrolPoint = FirstPatrolPoint;
    }
    else
    {
        CurrentPatrolPoint = SecondPatrolPoint;
    }

    UNavigationSystem::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bPatrol)
    {
        if (CurrentPatrolPoint)
        {
            FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
            const float Distance = Delta.Size();
            if (Distance <= 100)
                MoveToNextPatrolPoint();
        }
    }
}
