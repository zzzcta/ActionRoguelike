// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SAICharacter.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UDataTable;
class USMonsterData;
class USSaveGame;
class ASPickUpBase;
class UEnvQuery;
class UCurveFloat;
class UEnvQueryInstanceBlueprintWrapper;


USTRUCT(BlueprintType)
struct FMonsterDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterDataRow()
	{
		Weight = 1.0f;
		CoinsReward = 0.0f;
		SpawnCost = 0.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USMonsterData* MonsterData{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CoinsReward{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost{};
};

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
	UPROPERTY(EditDefaultsOnly)
	UDataTable* MonsterTable{};

	FString SaveGameName{"SaveGame01"};

	UPROPERTY()
	USSaveGame* CurrentSaveGame{};

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval{2.0f};

	// UPROPERTY(EditDefaultsOnly, Category = "AI")
	// TSubclassOf<AActor> MinionClass{};

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

	UFUNCTION(BlueprintCallable)
	void WriteSaveGame();

	void LoadSaveGame();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	FTimerHandle TimerHandle_SpawnBot;

public:
	virtual void OnActorKilled(AActor* ActorKilled, AActor* ActorKiller);
};
