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
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Awareness")
	class UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, Category="Awareness")
	class UAISenseConfig_Sight* SightConfig;

public:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, Category="Awareness")
	UObject* Target;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& Actors);
	
	
};
