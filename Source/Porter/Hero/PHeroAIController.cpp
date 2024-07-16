// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"

APHeroAIController::APHeroAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AddOwnedComponent(AIPerception);

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &APHeroAIController::OnPerceptionUpdated);
}

void APHeroAIController::BeginPlay()
{
	Super::BeginPlay();
	
}


void APHeroAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APHeroAIController::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{
	for (AActor* Actor : Actors)
	{
		UE_LOG(LogTemp, Log, TEXT("타겟이 시야 내에 있습니다: %s"), *Actor->GetName());
	}
}


void APHeroAIController::SetSightConfig(float SightRadius, float LoseSightRadius, float PeripheralVisionAngle) const
{
	if (SightConfig)
	{
		SightConfig->SightRadius = SightRadius;
		SightConfig->LoseSightRadius = LoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle;

		AIPerception->ConfigureSense(*SightConfig);
	}
}
