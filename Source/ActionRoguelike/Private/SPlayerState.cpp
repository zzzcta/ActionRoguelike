// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"

void ASPlayerState::AddCredits(const int32 Amount)
{
	Credits += Amount;
	OnCreditsChanged.Broadcast(Credits);
}

void ASPlayerState::SubtractCredits(const int32 Amount)
{
	Credits -= Amount;
	OnCreditsChanged.Broadcast(Credits);
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;
	}
}
