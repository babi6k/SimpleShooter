// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* pawnKilled) 
{
    Super::PawnKilled(pawnKilled);
    
    APlayerController* playerController = Cast<APlayerController>(pawnKilled->GetController());
    if (playerController != nullptr)
    {
        EndGame(false);
    }

    for (AShooterAIController* controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!controller->IsDead()) 
        {
            return;
        }
    }
    
    EndGame(true);

    //for loop over shooterAI in the world
        //Is not Dead ?
            //If 1 AI is alive return
    //No living AI End the game 
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner) 
{
    for (AController* controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = controller->IsPlayerController() == bIsPlayerWinner;
        controller->GameHasEnded(controller->GetPawn(), bIsWinner);
    }
}
