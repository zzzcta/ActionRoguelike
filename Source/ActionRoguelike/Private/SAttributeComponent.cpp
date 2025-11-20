// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = MaxHealth;
}


bool USAttributeComponent::Kill()
{
	return ApplyHealthChange(nullptr, -GetMaxHealth());
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	
	float ActualDelta = Health - OldHealth;
	
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	
	return ActualDelta != 0.0f;
}

USAttributeComponent* USAttributeComponent::GetAttributeComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributeComponent(Actor);
	
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	
	return false;
}




