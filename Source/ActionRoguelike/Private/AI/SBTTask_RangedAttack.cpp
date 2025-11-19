// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "SProjectileBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyController == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		ACharacter* TargetActor = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}
		
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator Rotation = Direction.Rotation();
		Rotation.Pitch += FMath::FRandRange(0.0f, MaxBulletSpreed);
		Rotation.Yaw += FMath::FRandRange(-MaxBulletSpreed, MaxBulletSpreed);
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = MyPawn;
		
		AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(Projectile, MuzzleLocation, Rotation, SpawnParameters);
		
		return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		
	}
	return EBTNodeResult::Failed;
}
