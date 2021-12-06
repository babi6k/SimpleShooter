// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(root);
}

void AGun::PullTrigger() 
{
	UGameplayStatics::SpawnEmitterAttached(muzzleFlash, mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(muzzleSound, mesh, TEXT("MuzzleFlashSocket"));

	FHitResult hit;
	FVector shotDirection;
	bool bSuccess = GunTrace(hit, shotDirection);
	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffect, hit.Location, shotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), impactSound, hit.Location);

		AActor* hitActor = hit.GetActor();
		if (hitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(damage, hit, shotDirection, nullptr);
			AController* ownerController = GetOwnerController();
			hitActor->TakeDamage(damage, DamageEvent, ownerController, this);
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult & hit, FVector& shotDirection) 
{
	AController* ownerController = GetOwnerController();
	if (ownerController == nullptr) return false;

	FVector location = FVector::ZeroVector;
	FRotator rotation = FRotator::ZeroRotator;

	ownerController->GetPlayerViewPoint(location, rotation);
	shotDirection = -rotation.Vector();
	FVector end = location + rotation.Vector() * maxRange;

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(hit, location, end , ECollisionChannel::ECC_GameTraceChannel1, params);
}

AController* AGun::GetOwnerController() const
{
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (ownerPawn == nullptr) return nullptr;
	return ownerPawn->GetController();
}

