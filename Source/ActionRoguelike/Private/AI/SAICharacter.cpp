// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		if (NewHealth <= OwningComp->GetMaxHealth() * 0.15)
		{
			AIC->GetBlackboardComponent()->SetValueAsBool("IsOnLowHealth", true);
		} else
		{
			AIC->GetBlackboardComponent()->SetValueAsBool("IsOnLowHealth", false);
		}
	}
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		
		BBComp->SetValueAsObject("TargetActor", Pawn);
		
		DrawDebugString(GetWorld(), GetActorLocation(), "Player Spotted", nullptr, FColor::White, 3.0f, true);
	}
}

