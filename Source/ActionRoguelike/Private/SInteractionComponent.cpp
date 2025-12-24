#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"
#include "SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugInteraction(TEXT("su.DebugInteraction"), false, TEXT("Enable debug interaction."), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FindInteractable();
}

void USInteractionComponent::FindInteractable()
{
	const bool bDebugInteraction = CVarDebugInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	
	TArray<FHitResult> OutHits;
	
	FVector EyeLocation;
	FRotator EyeRotation;

	const AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	const FVector End = EyeLocation + EyeRotation.Vector() * TraceLenght;

	FCollisionShape Sphere;
	Sphere.SetSphere(Radius);
	
	const bool bBlockingHit = GetWorld()->SweepMultiByObjectType(OutHits, EyeLocation, End, FQuat::Identity,
	ObjectQueryParams,Sphere);
	
	const FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	FocusedActor = nullptr;
	
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
				FocusedActor = HitActor;
				break;
			}
		}
	}
	
	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
	if (bDebugInteraction)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 5.0f);
	}
}


void USInteractionComponent::PrimaryInteract()
{
	if (FocusedActor)
	{
		APawn* Instigator = Cast<APawn>(GetOwner());
		ISGameplayInterface::Execute_Interact(FocusedActor, Instigator);
	}
}


