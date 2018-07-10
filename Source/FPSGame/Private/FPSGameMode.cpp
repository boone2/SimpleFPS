// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    // use our custom HUD class
    HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
    if (InstigatorPawn)
    {
        InstigatorPawn->DisableInput(nullptr);

        TArray<AActor*> ReturnedActors;
        UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);
        if (ReturnedActors.Num() > 0)
        {
            auto* PC = Cast<APlayerController>(InstigatorPawn->GetController());
            if (PC)
            {
                PC->SetViewTargetWithBlend(ReturnedActors[0], 0.5f, VTBlend_Cubic);
            }
        }
    }
    OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}
