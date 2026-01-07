// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "SAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> ActionClass : DefaultActionsClasses)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}


void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// FString DebugString = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugString);


	// Draw All Actions


	for (const USAction* Action : Actions)
	{
		const FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		const FString ActionMsg = FString::Printf(
			TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *Action->ActionName.ToString());

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}


TArray<USAction*> USActionComponent::GetActions()
{
	return Actions;
}


TArray<TSubclassOf<USAction>> USActionComponent::GetDefaultActionClasses()
{
	return DefaultActionsClasses;
}


void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);

	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
		NewAction->Initialize(this);

		if (NewAction->bAutoStart && ensure(NewAction->CanStartAction(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}


void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionClassName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionClassName)
		{
			if (!Action->CanStartAction(Instigator))
			{
				FString FailedMsj = FString::Printf(TEXT("Failed to run action: %s"), *ActionClassName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsj);
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				ServerStartActionByName(Instigator, ActionClassName);
			}

			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionClassName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionClassName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

float USActionComponent::GetActionRageCost(const FName ActionClassName)
{
	for (const USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionClassName)
		{
			return Action->RageCost;
		}
	}
	return 0.0f;
}


void USActionComponent::ServerStartActionByName_Implementation(AActor* Instigator, FName ActionClassName)
{
	StartActionByName(Instigator, ActionClassName);
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}

bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
                                            FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (USAction* Action : Actions)
	{
		WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
	}

	return WroteSomething;
}
