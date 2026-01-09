// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"
#include "SActionComponent.h"
#include "GameFramework/GameStateBase.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true;
}


void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	TimeStarted = GetWorld()->TimeSeconds;

	if (Duration > 0.0f)
	{
		FTimerDelegate DurationDelegate;
		DurationDelegate.BindUFunction(this, "StopAction", Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, DurationDelegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate PeriodDelegate;
		PeriodDelegate.BindUFunction(this, "ExecutePeriodEffect", Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodTimerHandle, PeriodDelegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodTimerHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodEffect_Implementation(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodTimerHandle);

	if (USActionComponent* ActionComp = GetOwningComponent())
	{
		ActionComp->RemoveAction(this);
	}
}

void USActionEffect::ExecutePeriodEffect_Implementation(AActor* Instigator)
{
}


float USActionEffect::GetTimeRemaining() const
{
	if (const AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>())
	{
		const float EndTime = TimeStarted + Duration;
		return EndTime - GameState->GetServerWorldTimeSeconds();
	}

	return Duration;
}
