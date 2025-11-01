// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractionComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontage;
class ASProjectile;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(EditAnywhere, Category = "Combat|Projectiles")
	TSubclassOf<ASProjectile> MagicProjectile;

	UPROPERTY(EditAnywhere, Category = "Combat|Projectiles")
	TSubclassOf<ASProjectile> BlackHoleProjectile;

	UPROPERTY(EditAnywhere, Category = "Combat|Animations")
	UAnimMontage* MagicAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Combat|Animations")
	UAnimMontage* BlackHoleAttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
	
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CamaraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void MagicAttack();
	void BlackHoleAttack();
	void FireProjectile(TSubclassOf<ASProjectile> Projectile);
	void PrimaryInteract();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
