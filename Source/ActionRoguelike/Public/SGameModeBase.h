// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

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
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval{2.0f};
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* BotSpawnQuery{};
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass{};
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UFUNCTION()
	void OnSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
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
