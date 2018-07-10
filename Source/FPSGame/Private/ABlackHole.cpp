// Fill out your copyright notice in the Description page of Project Settings.

#include "ABlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AABlackHole::AABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));	
	SetRootComponent(MeshComp);
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> Components;
	SphereComp->GetOverlappingComponents(Components);

	for (UPrimitiveComponent *Component : Components)
	{
		if (!Component->IsSimulatingPhysics())
			continue;
		Component->AddRadialForce(GetActorLocation(), SphereComp->GetScaledSphereRadius(), -2000, RIF_Constant, true);
	}
}

