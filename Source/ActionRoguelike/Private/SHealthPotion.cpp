#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"


#define LOCTEXT_NAMESPACE "InteractableActors"

void ASHealthPotion::Interact_Implementation(APawn* PickUpInstigator)
{
	Super::Interact_Implementation(PickUpInstigator);


	if (const USAttributeComponent* AttributeComponent = PickUpInstigator->FindComponentByClass<USAttributeComponent>())
	{
		bCanHeal = AttributeComponent->GetCurrentHealth() + HealthAmount < AttributeComponent->GetMaxHealth();
	}

	if (const ASPlayerState* PlayerState = Cast<ASPlayerState>(PickUpInstigator->GetPlayerState())
	)
	{
		bCanBuy = PlayerState->GetCredits() >= CoinsPrice;
	}

	if (bCanHeal && bCanBuy)
	{
		OnPickUp(PickUpInstigator);
	}
}

void ASHealthPotion::OnPickUp_Implementation(APawn* PickUpInstigator)
{
	Super::OnPickUp_Implementation(PickUpInstigator);

	if (ASPlayerState* PlayerState = Cast<ASPlayerState>(PickUpInstigator->GetPlayerState())
	)
	{
		PlayerState->SubtractCredits(CoinsPrice);
	}

	if (USAttributeComponent* AttributeComponent = PickUpInstigator->FindComponentByClass<USAttributeComponent>())
	{
		AttributeComponent->ApplyHealthChange(this, HealthAmount);
	}
}

FText ASHealthPotion::GetMessageInteract_Implementation(APawn* PickUpInstigator)
{
	if (const USAttributeComponent* AttributeComponent = PickUpInstigator->FindComponentByClass<USAttributeComponent>())
	{
		if (AttributeComponent->GetCurrentHealth() + HealthAmount < AttributeComponent->GetMaxHealth())
		{
			return FText::Format(LOCTEXT("HealthPotion_InteractableMessage", "Cost {0} Credits"), CoinsPrice);
		}

		return FText::Format(LOCTEXT("HealthPotion_Warning", "Already at max health"), CoinsPrice);
	}
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE
