// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpBase.h"

#include "Net/UnrealNetwork.h"

ASPickUpBase::ASPickUpBase()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	bReplicates = true;
	bIsActive = true;
}


void ASPickUpBase::Interact_Implementation(APawn* PickUpInstigator)
{
	ISGameplayInterface::Interact_Implementation(PickUpInstigator);
}

FText ASPickUpBase::GetMessageInteract_Implementation(APawn* PickUpInstigator)
{
	return FText::GetEmpty();
}

void ASPickUpBase::OnPickUp_Implementation(APawn* PickUpInstigator)
{
	bIsActive = false;
	OnRep_IsActive();
	GetWorldTimerManager().SetTimer(TimerHandle_ReactivatePickUpDelay, this, &ASPickUpBase::ReactivatePickUp,
	                                ReactivateDelay, false);
}

void ASPickUpBase::ReactivatePickUp()
{
	bIsActive = true;
	OnRep_IsActive();
}

void ASPickUpBase::OnRep_IsActive()
{
	RootComponent->SetVisibility(bIsActive);
	SetActorEnableCollision(bIsActive);
}

void ASPickUpBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPickUpBase, bIsActive)
}
