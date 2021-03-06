// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200));
	RootComponent = OverlapComp;

	OverlapComp->SetHiddenInGame(false);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::OverlapHandle);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200);
	DecalComp->SetupAttachment(RootComponent);
}

void AFPSExtractionZone::OverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AFPSCharacter *Pawn = Cast<AFPSCharacter>(OtherActor);
	if (Pawn == nullptr)
		return;

	UE_LOG(LogTemp, Log, TEXT("Overlaped extraction zone!"));

	if (Pawn->bIsCarryingObjective)
	{
		AFPSGameMode *GameMode = GetWorld()->GetAuthGameMode<AFPSGameMode>();
		if (GameMode)
		{
			GameMode->CompleteMission(Pawn, true);
		}
	}
	else
	{
		if (ObjectiveMissionSound)
		{
			UGameplayStatics::PlaySound2D(this, ObjectiveMissionSound);
		}
	}
}
