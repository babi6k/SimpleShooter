// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"


void AShooterAIController::BeginPlay() 
{
    Super::BeginPlay();
    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);

        APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        APawn* ownerPawn = GetPawn();
        
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), ownerPawn->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds); 
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* controllerdCharacter = Cast<AShooterCharacter>(GetPawn());
    if (controllerdCharacter != nullptr)
    {
        return controllerdCharacter->IsDead();
    }
    return true;
}
