#include "SAttributeComponent.h"
#include "SGameModeBase.h"

USAttributeComponent::USAttributeComponent()
{
	Health = MaxHealth;

	SetIsReplicatedByDefault(true);
}


void USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, const float RageAmount)
{
	const float OldRage = CurrentRage;
	const float NewRage = FMath::Clamp(CurrentRage + RageAmount, 0.0f, MaxRage);
	const float ActualDelta = NewRage - OldRage;

	if (GetOwner()->HasAuthority())
	{
		CurrentRage = NewRage;

		if (ActualDelta != 0.0f)
		{
			MulticastRageChanged(InstigatorActor, CurrentRage, ActualDelta);
		}
	}
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	ApplyRageChange(InstigatorActor, -Delta);

	const float OldHealth = Health;
	const float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	const float ActualDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, NewHealth, ActualDelta);
		}

		// If actor died
		if (Health <= 0.0f && ActualDelta < 0.0f)
		{
			if (ASGameModeBase* Gm = GetWorld()->GetAuthGameMode<ASGameModeBase>())
			{
				Gm->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return ActualDelta != 0.0f;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0;
}

float USAttributeComponent::GetCurrentHealth() const
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
