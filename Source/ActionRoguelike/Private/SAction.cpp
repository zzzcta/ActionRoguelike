// Fill out your copyright notice in the Description page of Project Settings.

#include "SAction.h"
#include "SActionComponent.h"


bool USAction::CanStartAction_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	
	USActionComponent* Comp = GetOwningComponent();
	
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}
void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StartAction: %s"), *GetNameSafe(this));
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	
	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StopAction: %s"), *GetNameSafe(this));
	
	ensureAlways(bIsRunning);
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	
	bIsRunning = false;
}

UWorld* USAction::GetWorld() const
{
	USActionComponent* Comp = Cast<USActionComponent>(GetOuter());
	
	if (Comp)
	{
		return Comp->GetWorld();
	}
	
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}



bool USAction::IsRunning() const
{
	return bIsRunning;
}