// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SActionEffect.h"


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
		USActionComponent* ActionComponent = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		
		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTagExact(ParryTag))
		{
			ProjectileMovementComponent->Velocity = -ProjectileMovementComponent->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();
			
			if (ActionComponent)
			{
				ActionComponent->AddAction(GetInstigator(), BurningActionClass);
			}
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




