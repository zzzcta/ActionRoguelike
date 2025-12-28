// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUpBase.h"
#include "SGrantAction_PickUp.generated.h"

class USAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGrantAction_PickUp : public ASPickUpBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USAction> ActionToGrant{};
	
	void Interact_Implementation(APawn* PickUpInstigator) override;
	void OnPickUp_Implementation(APawn* PickUpInstigator) override;
};
