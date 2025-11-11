// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"

void ASHealthPotion::Interact_Implementation(APawn* PickUpInstigator)
{
	Super::Interact_Implementation(PickUpInstigator);

	AttributeComp = Cast<USAttributeComponent>(PickUpInstigator->GetComponentByClass(USAttributeComponent::StaticClass()));

	if ensure(AttributeComp)
	{
		bCanHeal = AttributeComp->GetHealth() + HealthAmount < AttributeComp->GetMaxHealth();
	}
	
	if (bIsActive && bCanHeal)
	{
		OnPickUp(PickUpInstigator);
		bIsActive = false;
	}
}

void ASHealthPotion::OnPickUp_Implementation(APawn* PickUpInstigator)
{
	Super::OnPickUp_Implementation(PickUpInstigator);
	
	if ensure(AttributeComp)
	{
		AttributeComp->ApplyHealthChange(HealthAmount);
	}
}


