#include "SGameModeBase.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPickUpBase.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."),
                                                ECVF_Cheat);

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	NewPlayer->GetPlayerState<ASPlayerState>()->LoadPlayerState(CurrentSaveGame);
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot, this, &ASGameModeBase::SpawnBotTimerElapsed,
	                                SpawnTimerInterval, true);

	UEnvQueryInstanceBlueprintWrapper* ItemSpawnQuery = UEnvQueryManager::RunEQSQuery(
		this, ItemsSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
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
	UEnvQueryInstanceBlueprintWrapper* SpawnBotQuery = UEnvQueryManager::RunEQSQuery(
		this, BotSpawnQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

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

void ASGameModeBase::OnSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                             EEnvQueryStatus::Type QueryStatus)
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

void ASGameModeBase::WriteSaveGame()
{
	for (int i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		if (ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]))
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;
		}
	}

	CurrentSaveGame->SavedActors.Empty();
	
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && Actor->Implements<USGameplayInterface>())
		{
			FActorSavedData ActorSavedData{};
			ActorSavedData.ActorName = Actor->GetName();
			ActorSavedData.ActorTransform = Actor->GetTransform();
			
			FMemoryWriter MemoryWriter{ActorSavedData.ByteData};
			FObjectAndNameAsStringProxyArchive Ar{MemoryWriter, true};
			Ar.ArIsSaveGame = true;
			
			Actor->Serialize(Ar);
			
			CurrentSaveGame->SavedActors.Add(ActorSavedData);
		}
	}


	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveGameName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameName, 0));
		if (!CurrentSaveGame)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load save game."));
		}
		UE_LOG(LogTemp, Log, TEXT("SaveGame %s loaded."), *SaveGameName);

		for (TActorIterator<AActor> It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor && Actor->Implements<USGameplayInterface>())
			{
				for (const FActorSavedData& ActorSavedData : CurrentSaveGame->SavedActors)
				{
					if (ActorSavedData.ActorName == Actor->GetName())
					{
						Actor->SetActorTransform(ActorSavedData.ActorTransform);
						
						FMemoryReader MemoryReader{ActorSavedData.ByteData};
						FObjectAndNameAsStringProxyArchive Ar{MemoryReader, true};
						Ar.ArIsSaveGame = true;
						
						Actor->Serialize(Ar);
						
						ISGameplayInterface::Execute_OnActorLoaded(Actor);
						
						break;
					}
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("SaveGame %s created."), *SaveGameName);
	}
}
