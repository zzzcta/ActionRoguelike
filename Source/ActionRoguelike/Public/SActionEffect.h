// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:
	USActionEffect();

	UFUNCTION(BlueprintCallable, Category = "Effect")
	float GetTimeRemaining() const;

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration{};

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float Period{};

	FTimerHandle PeriodTimerHandle{};
	FTimerHandle DurationTimerHandle{};

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodEffect(AActor* Instigator);
};
