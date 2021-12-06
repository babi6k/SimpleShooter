// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> loseScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> winScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> hudScreenClass;

	UPROPERTY(EditAnywhere)
	float restartDelay = 5;

	FTimerHandle restartTimer;
	
	UPROPERTY()
	UUserWidget* HUD;
};
