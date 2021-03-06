// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "StandardGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TINYDOCTORTEST_API AStandardGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AStandardGameMode();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void PauseGame();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void GameOver();

	// Call to win the game
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void GameWon();
	
};
