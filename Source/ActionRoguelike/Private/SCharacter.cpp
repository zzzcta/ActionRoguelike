// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// This include is added at the top of your SCharacter.cpp file
#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
	ActionComponent = CreateDefaultSubobject<USActionComponent>("ActionComponent");
	
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
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);

	// Combat
	PlayerInputComponent->BindAction("MagicAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("DashAbility", IE_Pressed, this, &ASCharacter::DashAbility);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed ,this, &ASCharacter::PrimaryInteract);
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());
	
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
		DisableInput(PlayerController);
		SetLifeSpan(5.0f);
	}
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

void ASCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, FName("Sprint"));
}

void ASCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, FName("Sprint"));
}

void ASCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::BlackHoleAttack()
{
	if (AttributeComponent->GetRage() >= ActionComponent->GetActionRageCost("BlackHole"))
	{
		ActionComponent->StartActionByName(this, "BlackHole");
		AttributeComponent->ApplyRageChange(this, -ActionComponent->GetActionRageCost("BlackHole"));
	}
}

void ASCharacter::DashAbility()
{
	ActionComponent->StartActionByName(this, "Dash");
}

void ASCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CamaraComp->GetComponentLocation();
}

void ASCharacter::SelfHealth(const float HealthAmount)
{
	AttributeComponent->ApplyHealthChange(this, HealthAmount);
}

