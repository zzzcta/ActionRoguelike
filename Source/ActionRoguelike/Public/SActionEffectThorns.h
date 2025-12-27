// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SActionEffectThorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffectThorns : public USActionEffect
{
	GENERATED_BODY()

public:
	USActionEffectThorns();

protected:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	
	UPROPERTY(EditDefaultsOnly)
	float FractionDamage{};
	
	UFUNCTION()
	void ThornEffect(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
};
