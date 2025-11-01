// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// This include is added at the top of your SCharacter.cpp file
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	
	CamaraComp = CreateDefaultSubobject<UCameraComponent>("CamaraComp");
	CamaraComp->SetupAttachment(SpringArmComp);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of the pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in the direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed ,this, &ASCharacter::Jump);

	// Combat
	PlayerInputComponent->BindAction("MagicAttack", IE_Pressed, this, &ASCharacter::MagicAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed ,this, &ASCharacter::PrimaryInteract);
}

void ASCharacter::MoveForward(float value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	AddMovementInput(ControlRotation.Vector(), value);
	
}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(RightVector, value);
}

void ASCharacter::MagicAttack()
{
	PlayAnimMontage(MagicAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, [this](){
		FireProjectile(MagicProjectile);
	}, 0.2f, false);
}

void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(BlackHoleAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, [this](){
		FireProjectile(BlackHoleProjectile);
	}, 0.2f, false);
}

void ASCharacter::FireProjectile(TSubclassOf<ASProjectile> Projectile)
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
    
	FVector CameraLocation = CamaraComp->GetComponentLocation();
	FRotator CameraRotation = CamaraComp->GetComponentRotation();
    
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + CameraRotation.Vector() * 5000.0f;
	
	TArray<FHitResult> HitResults;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
    
	FVector TargetLocation = TraceEnd;

	if (GetWorld()->LineTraceMultiByObjectType(HitResults, TraceStart, TraceEnd, ECC_Visibility))
	{
		TargetLocation = HitResults[0].Location;
	}
	
	FRotator SpawnRotator = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetLocation);
	
	FTransform SpawnTM = FTransform(SpawnRotator, HandLocation);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;
	
	GetWorld()->SpawnActor<ASProjectile>(Projectile, SpawnTM, SpawnParameters); 
}

void ASCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}