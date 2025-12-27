// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class UParticleSystemComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UCameraShakeBase;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeOuterRadius;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "VFX")
	UParticleSystem* ImpactVfx;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UParticleSystemComponent* ParticleComponent;
	
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();
	
public:
	virtual void PostInitializeComponents() override;
	
};
