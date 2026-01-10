// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SMonsterData.generated.h"

class USAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster")
	TSubclassOf<AActor> MonsterClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster")
	TArray<TSubclassOf<USAction>> MonsterActions{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	UTexture2D* MonsterIcon{nullptr};
	
};
