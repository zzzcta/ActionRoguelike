// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimInstance.h"

#include "SActionComponent.h"

void USAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (const AActor* Actor = GetOwningActor())
	{
		ActionComponent = Cast<USActionComponent>(Actor->GetComponentByClass(USActionComponent::StaticClass()));
	}
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag(FName("Status.Stunned"));
	if (ActionComponent)
	{
		bIsStunned = ActionComponent->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
