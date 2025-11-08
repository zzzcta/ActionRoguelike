// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"


ASMagicProjectile::ASMagicProjectile()
{
	SphereComponent->SetSphereRadius(20.0f);
	DamageAmount = 20.0f;
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
		UE_LOG(LogTemp, Warning, TEXT("MagicProjectile: Suscrito a OnComponentBeginOverlap"));
	}
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("MagicProjectile: OnActorOverlap llamado! OtherActor: %s"), *GetNameSafe(OtherActor));
	
	if (OtherActor && OtherActor != GetInstigator())
	{
		UE_LOG(LogTemp, Warning, TEXT("MagicProjectile: Pasó la verificación de Instigator"));
		
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("MagicProjectile: AttributeComponent encontrado, aplicando daño"));
			// minus in front of DamageAmount to apply the change as damage, not healing
			AttributeComp->ApplyHealthChange(-DamageAmount);
			
			// Only explode when we hit something valid
			Explode();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MagicProjectile: NO se encontró AttributeComponent en %s"), *GetNameSafe(OtherActor));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MagicProjectile: Overlap con Instigator ignorado"));
	}
}




