// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickUpBase.generated.h"

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASPickUpBase : public AActor , public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickUpBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact_Implementation(APawn* PickUpInstigator) override;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly)
	float ReactivateDelay{10.0f};

	UPROPERTY()
	bool bIsActive{true};

	UFUNCTION()
	void ReactivatePickUp();

	UFUNCTION(BlueprintNativeEvent)
	void OnPickUp(APawn* PickUpInstigator);
	virtual void OnPickUp_Implementation(APawn* PickUpInstigator);
	
	FTimerHandle TimerHandle_ReactivatePickUpDelay;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
