// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class USActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	bool bIsRunning;
	
	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Texture{};
	
	UPROPERTY(Replicated)
	USActionComponent* ActionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category = "Tags")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;
	
	UPROPERTY(Replicated)
	float TimeStarted{};
	
	UFUNCTION()
	void OnRep_RepData();

public:
	void Initialize(USActionComponent* ActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	float RageCost{};

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	virtual UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
