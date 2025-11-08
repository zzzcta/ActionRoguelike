// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "DrawDebugHelpers.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	AActor::PostInitializeComponents();
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::FireImpulse);
}

void ASExplosiveBarrel::FireImpulse(
	UPrimitiveComponent* Projectile, 
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (RadialForceComponent)
	{
		RadialForceComponent->FireImpulse();
		UE_LOG(LogTemp, Log, TEXT("OnActorHit in explosive barrel"));
		UE_LOG(LogTemp, Warning, TEXT("Other actor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->GetTimeSeconds());

		FString CombinedString = FString::Printf(TEXT("Hit %s"), *Hit.ImpactPoint.ToString());
		DrawDebugString(GetWorld(), Hit.Location, CombinedString, nullptr, FColor::Green, 0.5f);
	}
}



