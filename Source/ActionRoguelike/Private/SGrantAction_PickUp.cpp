// Fill out your copyright notice in the Description page of Project Settings.


#include "SGrantAction_PickUp.h"

#include "SAction.h"
#include "SActionComponent.h"

void ASGrantAction_PickUp::Interact_Implementation(APawn* PickUpInstigator)
{
	Super::Interact_Implementation(PickUpInstigator);
	
	if (PickUpInstigator && PickUpInstigator->FindComponentByClass<USActionComponent>())
	{
		OnPickUp(PickUpInstigator);
	}
}

void ASGrantAction_PickUp::OnPickUp_Implementation(APawn* PickUpInstigator)
{
	if (USActionComponent* PickUpInstigatorActionComp = PickUpInstigator->FindComponentByClass<USActionComponent>())
	{
		const TArray<TSubclassOf<USAction>> DefaultActionsInstigator = PickUpInstigatorActionComp->GetDefaultActionClasses();
		
		if (!DefaultActionsInstigator.FindByKey(ActionToGrant))
		{
			PickUpInstigatorActionComp->AddAction(PickUpInstigator, ActionToGrant);
			Super::OnPickUp_Implementation(PickUpInstigator);
		}
	}
}
