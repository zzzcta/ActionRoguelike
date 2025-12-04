// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugInteraction(TEXT("su.DebugInteraction"), false, TEXT("Enable debug interaction."), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
}

void USInteractionComponent::PrimaryInteract()
{
	bool bDebugInteraction = CVarDebugInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	// FHitResult OutHit;
	TArray<FHitResult> OutHits;
	
	FVector EyeLocation;
	FRotator EyeRotation;
	
	AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + EyeRotation.Vector() * 250;

	float Radius = 30.0f;
	FCollisionShape Sphere;
	Sphere.SetSphere(Radius);
	
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(OutHit, EyeLocation, End, ObjectQueryParams);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(OutHits, EyeLocation, End, FQuat::Identity ,ObjectQueryParams, Sphere);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult& Hit : OutHits)
	{
		if (bDebugInteraction)
		{
			DrawDebugSphere(GetWorld(), Hit.Location, Radius, 32, LineColor, false, 2.0f);
		}
		
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				APawn* Instigator = Cast<APawn>(MyOwner);
				ISGameplayInterface::Execute_Interact(HitActor, Instigator);
				break;
			}
		}
	}
	if (bDebugInteraction)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 5.0f);
	}
}
