// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

class USActionEffect;
class UAudioComponent;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	USceneComponent* MuzzleFlashAttatchComponent{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* MuzzleFlash{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount{20.0f};
	
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	UAudioComponent* FlightSound{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* ImpactSound{nullptr};
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
	ASMagicProjectile();

};