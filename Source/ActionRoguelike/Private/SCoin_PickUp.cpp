// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin_PickUp.h"
#include "SPlayerState.h"

void ASCoin_PickUp::Interact_Implementation(APawn* PickUpInstigator)
{
	Super::Interact_Implementation(PickUpInstigator);
	
	if (bIsActive)
	{
		OnPickUp(PickUpInstigator);
	}
}

void ASCoin_PickUp::OnPickUp_Implementation(APawn* PickUpInstigator)
{
	Super::OnPickUp_Implementation(PickUpInstigator);

	if (ASPlayerState* PlayerState = Cast<ASPlayerState>(PickUpInstigator->GetPlayerState()))
	{
		PlayerState->AddCredits(CoinsToEarn);
	}
}
