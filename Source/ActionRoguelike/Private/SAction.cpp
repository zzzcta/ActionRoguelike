// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StartAction: %s"), *GetNameSafe(this));
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StopAction: %s"), *GetNameSafe(this));
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
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