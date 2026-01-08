// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameModeBase.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

	ActionComponent = CreateDefaultSubobject<USActionComponent>("ActionComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* TargetActor)
{
	if (AAIController* Aic = Cast<AAIController>(GetController()))
	{
		Aic->GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetActor);
	}
}

UObject* ASAICharacter::GetTargetActor()
{
	if (AAIController* Aic = Cast<AAIController>(GetController()))
	{
		return Aic->GetBlackboardComponent()->GetValueAsObject("TargetActor");
	}
	return nullptr;
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (Pawn != GetTargetActor())
	{
		SetTargetActor(Pawn);
		MulticastOnPawnSeen();
	}
}

void ASAICharacter::MulticastOnPawnSeen_Implementation()
{
	if (PlayerSpottedWidgetClass && !ActivePlayerSpottedWidget)
	{
		ActivePlayerSpottedWidget = CreateWidget<USWorldUserWidget>(GetWorld(), PlayerSpottedWidgetClass);
		if (ActivePlayerSpottedWidget)
		{
			ActivePlayerSpottedWidget->AttachedActor = this;
			ActivePlayerSpottedWidget->AddToViewport();
		}
	}
	else
	{
		if (ActivePlayerSpottedWidget)
		{
			ActivePlayerSpottedWidget->RemoveFromParent();
		}
	}
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

		if (ActiveHealthBarWidget == nullptr)
		{
			ActiveHealthBarWidget = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBarWidget)
			{
				ActiveHealthBarWidget->AttachedActor = this;
				ActiveHealthBarWidget->AddToViewport();
			}
		}


		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());

		const bool bIsLowHealth = NewHealth <= OwningComp->GetMaxHealth() * 0.15f;

		if (AAIController* AIC = Cast<AAIController>(GetController()))
		{
			AIC->GetBlackboardComponent()->SetValueAsBool("IsOnLowHealth", bIsLowHealth);

			// Bot Died
			if (NewHealth <= 0.0f)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");

				ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
				if (GM)
				{
					GM->OnBotKilled(InstigatorActor, this, CoinsToEarn);
				}

				GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				GetMesh()->SetAllBodiesSimulatePhysics(true);

				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				GetCharacterMovement()->Deactivate();
				SetLifeSpan(5.0f);
			}
		}
	}
}
