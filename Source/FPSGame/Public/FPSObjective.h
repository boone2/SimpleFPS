// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjective.generated.h"

UCLASS()
class FPSGAME_API AFPSObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjective();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent *MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent *SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UParticleSystem *PickupFX;

	UFUNCTION(Category = "Effect")
	void PlayEffect();
	
public:	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
