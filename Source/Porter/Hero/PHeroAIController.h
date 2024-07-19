// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "PHeroAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORTER_API APHeroAIController : public AAIController
{
	GENERATED_BODY()

public:
	APHeroAIController();

	void SetSightConfig(float SightRadius, float LoseSightRadius, float PeripheralVisionAngle) const;

protected:
	UPROPERTY(VisibleAnywhere, Category="Awareness")
	class UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, Category="Awareness")
	class UAISenseConfig_Sight* SightConfig;

	float CalculateDistanceToActor(AActor* OtherActor) const; 

public:
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& Actors);
	
};
