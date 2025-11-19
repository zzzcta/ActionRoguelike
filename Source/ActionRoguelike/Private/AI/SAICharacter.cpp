// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
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

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* TargetActor)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetActor);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "Player Spotted", nullptr, FColor::White, 3.0f, true);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
		if (Delta < 0.f)
		{
			if (InstigatorActor != this)
			{
				SetTargetActor(InstigatorActor);
			}
			
			GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());
			
			const bool bIsLowHealth = NewHealth <= OwningComp->GetMaxHealth() * 0.15f; 
			
			AAIController* AIC = Cast<AAIController>(GetController());
			AIC->GetBlackboardComponent()->SetValueAsBool("IsOnLowHealth", bIsLowHealth);
			
			if (NewHealth <= 0.f)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
				
				GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				GetMesh()->SetAllBodiesSimulatePhysics(true);
				
				SetLifeSpan(5.0f);
			}
		}	
}



