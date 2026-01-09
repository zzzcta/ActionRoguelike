// Fill out your copyright notice in the Description page of Project Settings.

#include "SAction.h"
#include "SActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComponent = NewActionComp;
}

bool USAction::CanStartAction_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	const USActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("StartAction: %s"), *GetNameSafe(this));

	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("StopAction: %s"), *GetNameSafe(this));

	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

UWorld* USAction::GetWorld() const
{
	if (const AActor* Actor = Cast<AActor>(GetOuter()))
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComponent;
}

void USAction::OnRep_RepData()
{
	RepData.bIsRunning ? StartAction(RepData.Instigator) : StopAction(RepData.Instigator);
}

bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData)
	DOREPLIFETIME(USAction, ActionComponent)
	DOREPLIFETIME(USAction, TimeStarted)
}
