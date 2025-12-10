// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpBase.h"

ASPickUpBase::ASPickUpBase()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

void ASPickUpBase::Interact_Implementation(APawn* PickUpInstigator)
{
	ISGameplayInterface::Interact_Implementation(PickUpInstigator);
}

void ASPickUpBase::OnPickUp_Implementation(APawn* PickUpInstigator)
{
	BaseMesh->SetVisibility(false);
	GetWorldTimerManager().SetTimer(TimerHandle_ReactivatePickUpDelay, this, &ASPickUpBase::ReactivatePickUp, ReactivateDelay, false);
}

void ASPickUpBase::ReactivatePickUp()
{
	BaseMesh->SetVisibility(true);
	bIsActive = true;
}

