// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

APHeroAIController::APHeroAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 0;
	SightConfig->LoseSightRadius = 0;
	SightConfig->PeripheralVisionAngleDegrees = 0;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AddOwnedComponent(AIPerception);
}

void APHeroAIController::BeginPlay()
{
	Super::BeginPlay();
}

void APHeroAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APHeroAIController::SetSightConfig(float SightRadius, float LoseSightRadius, float PeripheralVisionAngle) const
{
	UE_LOG(LogTemp, Log, TEXT("Sight Radius :: %f"), SightRadius);
	if (SightConfig)
	{
		SightConfig->SightRadius = SightRadius;
		SightConfig->LoseSightRadius = LoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle;
	}
}
