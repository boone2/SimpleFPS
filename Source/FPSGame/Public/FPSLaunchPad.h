// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent *MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent *OverlapComp;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
	class UParticleSystem *LaunchEffect;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
	float LaunchStrength;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
	float LaunchPitch;

	UFUNCTION()
	void OverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
