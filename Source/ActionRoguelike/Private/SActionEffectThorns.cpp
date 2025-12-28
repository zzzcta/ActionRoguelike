// Fill out your copyright notice in the Description page of Project Settings.

#include "SActionEffectThorns.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"

USActionEffectThorns::USActionEffectThorns()
{
	Duration = TNumericLimits<float>::Max();
}

void USActionEffectThorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (const USActionComponent* ActionComp = GetOwningComponent())
	{
		if (const AActor* OwnerActor = ActionComp->GetOwner())
		{
			if (USAttributeComponent* OwnerAttributeComp = OwnerActor->FindComponentByClass<USAttributeComponent>())
			{
				OwnerAttributeComp->OnHealthChanged.AddDynamic(this, &USActionEffectThorns::ThornEffect);	
			}
		}
	}
}

void USActionEffectThorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	
	if (const USActionComponent* ActionComp = GetOwningComponent())
	{
		if (const AActor* OwnerActor = ActionComp->GetOwner())
		{
			if (USAttributeComponent* OwnerAttributeComp = OwnerActor->FindComponentByClass<USAttributeComponent>())
			{
				OwnerAttributeComp->OnHealthChanged.RemoveDynamic(this, &USActionEffectThorns::ThornEffect);	
			}
		}
	}
}

void USActionEffectThorns::ThornEffect(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (const USActionComponent* ActionComp = GetOwningComponent())
	{
		if (AActor* OwnerActor = ActionComp->GetOwner())
		{
			if (InstigatorActor && InstigatorActor != OwnerActor)
			{
				if (USAttributeComponent* InstigatorAttributeComp = InstigatorActor->FindComponentByClass<USAttributeComponent>())
				{
					InstigatorAttributeComp->ApplyHealthChange(OwnerActor, Delta / FractionDamage);
				}
			}
		}
	}
}