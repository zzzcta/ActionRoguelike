// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectile
{
	public:
		ASDashProjectile();

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	
		UPROPERTY(EditDefaultsOnly)
		UParticleSystem* TeleportEnterParticle;

		UPROPERTY(EditDefaultsOnly)
		UParticleSystem* TeleportExitParticle;
	
		UPROPERTY(EditDefaultsOnly)
		float ExplodeTimer{0.2f};

		UFUNCTION()
		void OnExplodeTimer();
	
		UFUNCTION()
		void TeleportInstigator();
	
		FTimerHandle TimerHandle_ExplodeTimer;
	
	
	
	GENERATED_BODY()
	
};
