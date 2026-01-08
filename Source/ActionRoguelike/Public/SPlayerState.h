#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsAmountChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

/**
 * 
 */

class USSaveGame;

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(ReplicatedUsing="OnRep_Credits")
	int32 Credits{};
	
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddCredits(int32 Amount);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SubtractCredits(int32 Amount);

	UFUNCTION(BlueprintPure)
	int32 GetCredits() const;

	UPROPERTY(BlueprintAssignable)
	FOnCreditsAmountChanged OnCreditsChanged;
	
	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);
	
	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);
};
