// Fill out your copyright notice in the Description page of Project Settings.

#include "TinyDoctorTest.h"
#include "MinionSpawner.h"

#include "BossAIController.h"

// Sets default values
AMinionSpawner::AMinionSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void AMinionSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// Adds itself to the boss' minionSpawner-array.
	Cast<ABossAIController>(bossReference->GetController())->spawners.Add(this);
}

// Called every frame
void AMinionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMinionSpawner::StartSpawning_Implementation(const int amountToSpawn)
{
	// Not designed to use C++. Use Blueprint-derived version.
}

