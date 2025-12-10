// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUpBase.h"
#include "SHealthPotion.generated.h"

class USAttributeComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickUpBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	USAttributeComponent* AttributeComp{};

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float HealthAmount{10.0f};

	UPROPERTY()
	bool bCanHeal;
	
	UPROPERTY()
	bool bCanBuy;
	
	UPROPERTY(EditDefaultsOnly, Category = "Coins")
	int32 CoinsPrice{};
	
	virtual void OnPickUp_Implementation(APawn* PickUpInstigator) override;
	virtual void Interact_Implementation(APawn* PickUpInstigator) override;
};
