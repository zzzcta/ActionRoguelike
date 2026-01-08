// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickUpBase.generated.h"

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASPickUpBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	ASPickUpBase();

protected:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_IsActive)
	bool bIsActive{};
	
	UFUNCTION()
	void OnRep_IsActive();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BaseMesh{};

	UPROPERTY(EditDefaultsOnly)
	float ReactivateDelay{10.0f};

	UFUNCTION()
	void ReactivatePickUp();

	UFUNCTION(BlueprintNativeEvent)
	void OnPickUp(APawn* PickUpInstigator);

	virtual void Interact_Implementation(APawn* PickUpInstigator) override;

	FTimerHandle TimerHandle_ReactivatePickUpDelay;
};
