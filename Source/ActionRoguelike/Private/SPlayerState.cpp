// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

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
