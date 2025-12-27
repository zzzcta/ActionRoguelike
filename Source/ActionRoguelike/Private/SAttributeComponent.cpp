#include "SAttributeComponent.h"
#include "SGameModeBase.h"

USAttributeComponent::USAttributeComponent()
{
	Health = MaxHealth;
}

bool USAttributeComponent::Kill()
{
	return ApplyHealthChange(nullptr, -GetMaxHealth());
}

float USAttributeComponent::GetRage()
{
	return CurrentRage;
}

float USAttributeComponent::GetMaxRage()
{
	return MaxHealth;
}

void USAttributeComponent::ApplyRageChange(const float RageAmount)
{
	const float OldRage = CurrentRage;
	CurrentRage = FMath::Clamp(CurrentRage + RageAmount , 0.0f, MaxRage);
	const float ActualDelta = CurrentRage - OldRage;
	OnRageChanged.Broadcast(this, CurrentRage, ActualDelta);
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
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}
	
	ApplyRageChange(-Delta);
	
	const float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	const float ActualDelta = Health - OldHealth;
	
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	
	// If actor died
	if (Health <= 0.0f && ActualDelta < 0.0f)
	{
		if (ASGameModeBase* Gm = GetWorld()->GetAuthGameMode<ASGameModeBase>())
		{
			Gm->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
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
	if (const USAttributeComponent* AttributeComp = GetAttributeComponent(Actor))
	{
		return AttributeComp->IsAlive();
	}
	
	return false;
}




