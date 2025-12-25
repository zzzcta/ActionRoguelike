// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USInteractionComponent();
	
protected:
	
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);
	
	UPROPERTY()
	AActor* FocusedActor{};
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float Radius{30.0f};
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float TraceLenght{500.0f};
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass{nullptr};
	
	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	TEnumAsByte<ECollisionChannel> CollisionChannel{ECC_WorldDynamic};
	
	UFUNCTION()
	void FindInteractable();
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PrimaryInteract();
	
};
