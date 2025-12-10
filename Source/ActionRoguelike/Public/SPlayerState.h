#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsAmountChanged, int32, Credits);
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	int32 Credits{};
	
public:
	UFUNCTION(BlueprintCallable)
	void AddCredits(int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	void SubtractCredits(int32 Amount);
	
	UFUNCTION(BlueprintPure)
	int32 GetCredits() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnCreditsAmountChanged OnCreditsChanged;
};
	