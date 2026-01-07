// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASItemChest();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	virtual void OnActorLoaded_Implementation() override;

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing="OnRep_LidOpened", SaveGame)
	bool bIsLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UStaticMeshComponent* LidMesh;
};
