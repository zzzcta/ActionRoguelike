// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

class USActionEffect;
class UAudioComponent;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<USActionEffect> BurningActionClass{};

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount{20.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	UAudioComponent* FlightSound{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* ImpactSound{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	FGameplayTag ParryTag;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void Explode_Implementation() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	ASMagicProjectile();
};
