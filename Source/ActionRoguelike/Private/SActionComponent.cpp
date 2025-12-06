// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TSubclassOf<USAction> ActionClass : DefaultActionsClasses)
	{
		AddAction(ActionClass);
	}
	
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FString DebugString = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, DebugString);
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	
	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionClassName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionClassName)
		{
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
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}
