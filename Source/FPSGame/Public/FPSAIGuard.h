// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
    Idle,
    Suspicious,
    Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AFPSAIGuard();

protected:
    // Called when the game starts or when spawned
    void BeginPlay() override;

public:

    // Called every frame
    void Tick(float DeltaTime) override;

private:
    FRotator OriginalRotation;
    FTimerHandle TimerHandle_ResetRotation;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UPawnSensingComponent* PawnSensingComp;

    UFUNCTION()
    void OnPawnSeen(APawn* Pawn);

    UFUNCTION()
    void OnNoiseHeard(APawn* InstigatorPawn, const FVector& Location, float Volume);

    UFUNCTION()
    void ResetRotation();

    // AIState
private:
    EAIState GuardState;

    void SetGuardState(EAIState NewState);

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "AI")
    void OnStateChanged(EAIState NewState);

    // Patrol
private:
    UPROPERTY(EditInstanceOnly, Category = "AI")
    bool bPatrol;

    UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
    AActor* FirstPatrolPoint;

    UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
    AActor* SecondPatrolPoint;

    AActor* CurrentPatrolPoint;

    void MoveToNextPatrolPoint();
};
