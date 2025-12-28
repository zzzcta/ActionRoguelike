// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	
	if (ensure(AIC))
	{
		ACharacter* MyCharacter = Cast<ACharacter>(AIC->GetPawn());
		if (MyCharacter)
		{
			USAttributeComponent* MyCharacterAttributeComp = Cast<USAttributeComponent>(MyCharacter->GetComponentByClass(USAttributeComponent::StaticClass()));
			if (MyCharacterAttributeComp)
			{
				MyCharacterAttributeComp->ApplyHealthChange(nullptr, MyCharacterAttributeComp->GetMaxHealth());
				DrawDebugString(GetWorld(), MyCharacter->GetActorLocation(), "Curita pls", nullptr, FColor::Yellow, 1.5f);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
