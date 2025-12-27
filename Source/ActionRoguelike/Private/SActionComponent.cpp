// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TSubclassOf<USAction> ActionClass : DefaultActionsClasses)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FString DebugString = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugString);
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	
	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
		
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
