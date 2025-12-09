// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USActionComponent;
class USWorldUserWidget;
class USAttributeComponent;
class UPawnSensingComponent;
class UUserWidget;
UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USAttributeComponent* AttributeComponent{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USActionComponent* ActionComponent{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	USWorldUserWidget* ActiveHealthBarWidget{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass{};
	
	UFUNCTION()
	void SetTargetActor(AActor* TargetActor);
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
	

	
};
