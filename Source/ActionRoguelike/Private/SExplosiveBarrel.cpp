// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "DrawDebugHelpers.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
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

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

