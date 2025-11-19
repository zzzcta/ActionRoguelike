// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


ASMagicProjectile::ASMagicProjectile()
{
	SphereComponent->SetSphereRadius(20.0f);
	FlightSound = CreateDefaultSubobject<UAudioComponent>("FlightSound");
	FlightSound->SetupAttachment(SphereComponent);
	
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if ensure(SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	}

	SpawnMuzzleFlash();
	
}

void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			// minus in front of DamageAmount to apply the change as damage, not healing
			AttributeComp->ApplyHealthChange(GetInstigator() ,-DamageAmount);
			
			// Only explode when we hit something valid
			Explode();
		}
	}
}

void ASMagicProjectile::SpawnMuzzleFlash() const
{
	const ACharacter* InstigatorCharacter = Cast<ACharacter>(GetInstigator());
	
	if ensure(InstigatorCharacter && MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(
			MuzzleFlash, 
			InstigatorCharacter->GetMesh(), 
			FName("Muzzle_01"),           
			FVector::ZeroVector,            
			FRotator::ZeroRotator,         
			EAttachLocation::SnapToTarget,  
			true                           
		);
	}
}

void ASMagicProjectile::Explode_Implementation()
{
	Super::Explode_Implementation();
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
}




