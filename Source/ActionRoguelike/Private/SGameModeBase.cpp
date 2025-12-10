#include "SGameModeBase.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPickUpBase.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	UEnvQueryInstanceBlueprintWrapper* ItemSpawnQuery = UEnvQueryManager::RunEQSQuery(this, ItemsSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	if (ensure(ItemSpawnQuery))
	{
		ItemSpawnQuery->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnItemSpawnQueryFinished);
	}
}

void ASGameModeBase::OnBotKilled(AActor* Killer, ASAICharacter* BotKilled, const int32 CoinsToEarn)
{
	const ASCharacter* Player = Cast<ASCharacter>(Killer);
	if (Player)
	{
		ASPlayerState* PlayerState = Cast<ASPlayerState>(Player->GetPlayerState());
		if (ensure(PlayerState))
		{
			PlayerState->AddCredits(CoinsToEarn);
		}
	}
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

TSubclassOf<ASPickUpBase> ASGameModeBase::GetRandomItemClass() const
{
	if (ItemsToSpawn.Num() == 0)
	{
		return nullptr;
	}
	
	return ItemsToSpawn[FMath::RandRange(0, ItemsToSpawn.Num() - 1)];
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

void ASGameModeBase::OnItemSpawnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed Query"));
		return;
	}
	
	TArray<FVector> SpawnLocations{};
	QueryInstance->GetQueryResultsAsLocations(SpawnLocations);
	
	if (SpawnLocations.Num() == 0)
	{
		return;
	}

	for (int i = 0; i <= MaxItemsToSpawn; ++i)
	{
		int32 RandomIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
		GetWorld()->SpawnActor<AActor>(GetRandomItemClass(), SpawnLocations[RandomIndex], FRotator::ZeroRotator);
		SpawnLocations.RemoveAt(RandomIndex);
	}
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