#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

void ASHealthPotion::Interact_Implementation(APawn* PickUpInstigator)
{
	Super::Interact_Implementation(PickUpInstigator);

	AttributeComp = Cast<USAttributeComponent>(PickUpInstigator->GetComponentByClass(USAttributeComponent::StaticClass()));

	if ensure(AttributeComp)
	{
		bCanHeal = AttributeComp->GetHealth() + HealthAmount < AttributeComp->GetMaxHealth();
	}
	
	ASPlayerState* PlayerState = Cast<ASPlayerState>(PickUpInstigator->GetPlayerState());
	if (ensure(PlayerState))
	{
		bCanBuy = PlayerState->GetCredits() >= CoinsPrice;
	}
	
	if (bIsActive && bCanHeal && bCanBuy)
	{
		OnPickUp(PickUpInstigator);
		bIsActive = false;
	}
}

void ASHealthPotion::OnPickUp_Implementation(APawn* PickUpInstigator)
{
	Super::OnPickUp_Implementation(PickUpInstigator);
	
	ASPlayerState* PlayerState = Cast<ASPlayerState>(PickUpInstigator->GetPlayerState());
	if (PlayerState)
	{
		PlayerState->SubtractCredits(CoinsPrice);
	}
	
	if ensure(AttributeComp)
	{
		AttributeComp->ApplyHealthChange(this, HealthAmount);
	}
}


