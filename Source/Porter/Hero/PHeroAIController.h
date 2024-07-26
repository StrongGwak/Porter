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

	// 시야 감지 설정 함수
	void SetSightConfig(float SightRadius, float LoseSightRadius, float PeripheralVisionAngle) const;

protected:
	// 감지 컴포넌트
	UPROPERTY(VisibleAnywhere, Category="Awareness")
	class UAIPerceptionComponent* AIPerception;

	// 시야 감지 기능
	UPROPERTY(VisibleAnywhere, Category="Awareness")
	class UAISenseConfig_Sight* SightConfig;

	// 액터간 거리 계산 함수
	float CalculateDistanceToActor(AActor* OtherActor) const;

public:
	virtual void OnPossess(APawn* InPawn) override;

	// 감지 여부마다 실행되는 함수
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& Actors);
	
};
