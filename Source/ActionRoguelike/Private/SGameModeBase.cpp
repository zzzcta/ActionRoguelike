// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);
void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* SpawnBotQuery = UEnvQueryManager::RunEQSQuery(this, BotSpawnQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	
	if (ensure(SpawnBotQuery))
	{
		SpawnBotQuery->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnBotQueryFinished);
	}
}

void ASGameModeBase::KillMinions()
{
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComponent(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill();
		}
	}
}


void ASGameModeBase::OnSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed Query"));
		return;
	}
	
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnBots is disabled."));
		return;
	}
	
	// Count alive bots before spawning
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComponent(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 10.0f;
	
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}
	
	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	
	if (SpawnLocations.Num() == 0)
	{
		return;
	}
	
	GetWorld()->SpawnActor<AActor>(MinionClass, SpawnLocations[0], FRotator::ZeroRotator);
	
}

void ASGameModeBase::OnActorKilled(AActor* ActorKilled, AActor* ActorKiller)
{
	ASCharacter* Player = Cast<ASCharacter>(ActorKilled);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, FName("RespawnPlayerElapsed"), Player->GetController());
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, 2.0f, false);
	}

}

void ASGameModeBase::RespawnPlayerElapsed(AController* PlayerController)
{
	if (ensure(PlayerController))
	{
		PlayerController->UnPossess();
		
		RestartPlayer(PlayerController);
	}
}