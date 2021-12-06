// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	health = maxHealth;

	gun = GetWorld()->SpawnActor<AGun>(gunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	gun->SetOwner(this);
}

bool AShooterCharacter::IsDead() const
{
	return health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return health / maxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	float damageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	damageToApply = FMath::Min(health, damageToApply);
	health -= damageToApply;
	UE_LOG(LogTemp, Warning , TEXT("Health left : %f"), health);
	if (IsDead())
	{
		ASimpleShooterGameModeBase* gameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (gameMode != nullptr)
		{
			gameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	} 

	return damageToApply;	
}

void AShooterCharacter::MoveForward(float axisValue)
{
	if (axisValue <= 0.1f && axisValue >= -0.1f)
	{
		axisValue = 0.f;
	}
	AddMovementInput(GetActorForwardVector() * axisValue);
}

void AShooterCharacter::MoveRight(float axisValue)
{
	if (axisValue <= 0.1f && axisValue >= -0.1f)
	{
		axisValue = 0.f;
	}
	AddMovementInput(GetActorRightVector() * axisValue);
}

void AShooterCharacter::LookUpRate(float axisValue) 
{
	//UE_LOG(LogTemp, Warning , TEXT("Axis Value is : %f"), axisValue);
	if (axisValue <= 0.1f && axisValue >= -0.1f)
	{
		axisValue = 0.f;
	}
	AddControllerPitchInput(axisValue * rotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float axisValue) 
{
	if (axisValue <= 0.1f && axisValue >= -0.1f)
	{
		axisValue = 0.f;
	}
	AddControllerYawInput(axisValue * rotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot() 
{
	gun->PullTrigger();
}



