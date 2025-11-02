// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	TeleportEnterParticle = CreateDefaultSubobject<UParticleSystem>("TeleportEnterParticle");
}


void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_ExplodeTimer, this, &ASDashProjectile::OnExplodeTimer, ExplodeTimer, false);
}

void ASDashProjectile::OnExplodeTimer()
{
	ProjectileMovementComponent->StopMovementImmediately();
	ParticleComponent->SetTemplate(TeleportEnterParticle);
	ParticleComponent->Activate();
	GetWorldTimerManager().SetTimer(TimerHandle_ExplodeTimer, this, &ASDashProjectile::TeleportInstigator, ExplodeTimer, false);
}

void ASDashProjectile::TeleportInstigator()
{
	FRotator ProjectileRotation = this->GetActorRotation();
	
	FRotator CleanYawRotation = FRotator(0.0f, ProjectileRotation.Yaw, 0.0f);
	GetInstigator()->TeleportTo(this->GetActorLocation(), CleanYawRotation);
	ParticleComponent->SetTemplate(TeleportExitParticle);
	ParticleComponent->Activate();
	GetWorldTimerManager().SetTimer(TimerHandle_ExplodeTimer, [this]
	{
		Destroy();
	},ExplodeTimer, false);
}
