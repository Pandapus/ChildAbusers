// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.h"

#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class TINYDOCTORTEST_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	ABaseAIController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Holds reference to player1
	static AActor* playerReference;

	// Reference to the character the AIController is controlling.
	AUnit* characterReference;
	virtual void SetCharacterReference();

	// Position at which the enemy spawned on
	FVector originalPosition;

	// Gets relative vector from controlled enemy to player actor.
	UFUNCTION(BlueprintPure)
		const FVector GetVectorToPlayer();

	// Calculates distance from enemy to player
	UFUNCTION(BlueprintPure)
		const float DistanceToPlayer();

	// Moves characterReference to playerReference's position.
	void MoveToPlayer();
	
public:
	friend class AEnemy;
};
