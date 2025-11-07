// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	public:
		ASDashProjectile();

	protected:
	
		UPROPERTY(EditDefaultsOnly)
		UParticleSystem* TeleportEnterParticle;

		UPROPERTY(EditDefaultsOnly)
		UParticleSystem* TeleportExitParticle;
	
		UPROPERTY(EditDefaultsOnly)
		float ExplodeDelay{};

		UPROPERTY(EditDefaultsOnly)
		float TeleportDelay{};

		UPROPERTY(EditDefaultsOnly)
		float TeleportExitParticleDuration{};
	
		FTimerHandle TimerHandle_ExplodeDelay;

		// Base class using BlueprintNativeEvent, we must override the _Implementation not the Explode()
		virtual void Explode_Implementation() override;

		void TeleportInstigator();

		virtual void BeginPlay() override;
	
	GENERATED_BODY()
	
};
