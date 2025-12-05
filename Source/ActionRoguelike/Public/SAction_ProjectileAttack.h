// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class ASProjectileBase;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Combat|Projectiles")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Combat|Animations")
	UAnimMontage* AttackAnim;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat|Animations")
	FName HandSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Animations")
	float AttackAnimDelay;
	
	UPROPERTY(EditAnywhere, Category = "Combat|Animations")
	UParticleSystem* CastingEffect;
	
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
	
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	USAction_ProjectileAttack();
	
};
