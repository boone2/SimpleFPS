// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjective.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

// Sets default values
AFPSObjective::AFPSObjective()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MehsComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);	
}

// Called when the game starts or when spawned
void AFPSObjective::BeginPlay()
{
	Super::BeginPlay();
	
	PlayEffect();
}

void AFPSObjective::PlayEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupFX, GetActorTransform());
}

void AFPSObjective::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	PlayEffect();
	AFPSCharacter *FPSCharacter = Cast<AFPSCharacter>(OtherActor);
	if (FPSCharacter)
	{
		FPSCharacter->bIsCarryingObjective = true;
		Destroy();
	}
}

