// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "GameFramework/GameModeBase.h"
#include "Serialization/ArrayReader.h"
#include "SGameModeBase.generated.h"

class ASPickUpBase;
class UEnvQuery;
class UCurveFloat;
class UEnvQueryInstanceBlueprintWrapper;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;
	static void OnBotKilled(AActor* Killer, ASAICharacter* BotKilled, int32 CoinsToEarn);

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval{2.0f};
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass{};
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "Querys|IA")
	UEnvQuery* BotSpawnQuery{};
	
	UPROPERTY(EditDefaultsOnly, Category = "Querys|Items")
	UEnvQuery* ItemsSpawnQuery{};
	
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TArray<TSubclassOf<ASPickUpBase>> ItemsToSpawn{};
	
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	int32 MaxItemsToSpawn{6};
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	TSubclassOf<ASPickUpBase> GetRandomItemClass() const;
	
	UFUNCTION()
	void OnSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	UFUNCTION()
	void OnItemSpawnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	UFUNCTION()
	void SpawnBotTimerElapsed();
	
	UFUNCTION(Exec)
	void KillMinions();
	
	UFUNCTION()
	void RespawnPlayerElapsed(AController* PlayerController);
	
	FTimerHandle TimerHandle_SpawnBot;
	
public:
	virtual void OnActorKilled(AActor* ActorKilled, AActor* ActorKiller);
	
};
