// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionChanged, USActionComponent*, ActionComponent, USAction*, Action);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USActionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(USAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionClassName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionClassName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	float GetActionRageCost(FName ActionClassName);

	UFUNCTION(Server, Reliable)
	void ServerStartActionByName(AActor* Instigator, FName ActionClassName);

	UFUNCTION(Server, Reliable)
	void ServerStopActionByName(AActor* Instigator, FName ActionClassName);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	TArray<USAction*> GetActions();

	TArray<TSubclassOf<USAction>> GetDefaultActionClasses();

protected:
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<USAction>> DefaultActionsClasses;

	UPROPERTY(Replicated)
	TArray<USAction*> Actions;

	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnActionChanged OnActionStarted{};
	
	UPROPERTY(BlueprintAssignable)
	FOnActionChanged OnActionStopped{};
	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	                                 FReplicationFlags* RepFlags) override;
};
