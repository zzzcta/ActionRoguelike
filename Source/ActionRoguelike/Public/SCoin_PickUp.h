// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUpBase.h"
#include "SCoin_PickUp.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCoin_PickUp : public ASPickUpBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	int32 CoinsToEarn{10};

	virtual void Interact_Implementation(APawn* PickUpInstigator) override;

	virtual void OnPickUp_Implementation(APawn* PickUpInstigator) override;
};
