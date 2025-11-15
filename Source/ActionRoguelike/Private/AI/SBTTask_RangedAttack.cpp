// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
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
		
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator Rotation = Direction.Rotation();
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(Projectile, MuzzleLocation, Rotation, SpawnParameters);
		
		return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		
	}
	return EBTNodeResult::Failed;
}
