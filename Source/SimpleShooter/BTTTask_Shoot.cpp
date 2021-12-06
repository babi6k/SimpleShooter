// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTTask_Shoot.h"
#include "ShooterCharacter.h"
#include "AIController.h"


UBTTTask_Shoot::UBTTTask_Shoot() 
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AShooterCharacter* character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (character == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    character->Shoot();

    return EBTNodeResult::Succeeded;
}
