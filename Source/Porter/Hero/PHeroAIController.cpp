// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroAIController.h"

#include "PHero.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"

APHeroAIController::APHeroAIController()
{
	// AI Perception Component 생성
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	// 시각 생성
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// 인지 조건 설정
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// AI Perception에 시각 추가
	AIPerception->ConfigureSense(*SightConfig);
	// AI Perception Component 추가
	AddOwnedComponent(AIPerception);

	// 인지 함수 바인드
	AIPerception->OnPerceptionUpdated.AddDynamic(this, &APHeroAIController::OnPerceptionUpdated);
}

void APHeroAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}

void APHeroAIController::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{
	APHero* PHero = Cast<APHero>(GetPawn());
	TArray<AActor*> Knowns;
	AIPerception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), Knowns);
	if (!Knowns.IsEmpty())
	{
		AActor* Target = nullptr;
		float minDistance = SightConfig->SightRadius;
		for (AActor* Actor : Knowns)
		{
			float Distance = CalculateDistanceToActor(Actor);
			if (Distance < minDistance)
			{
				minDistance = Distance;
				Target = Actor;
				UE_LOG(LogTemp, Log, TEXT("가장 가까운 타겟: %s"), *Target->GetName());
			}
		}
		UE_LOG(LogTemp, Log, TEXT("타겟이 시야 내에 있습니다: %s"), *Target->GetName());
		PHero->FindTarget(Target);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("타겟이 사라졌습니다."));
		PHero->StopAttack();
	}
}

float APHeroAIController::CalculateDistanceToActor(AActor* OtherActor) const
{
	FVector MyLocation = GetPawn()->GetActorLocation();
	FVector OtherActorLocation = OtherActor->GetActorLocation();

	float Distance = FVector::Dist(MyLocation, OtherActorLocation);

	return Distance;
}

void APHeroAIController::SetSightConfig(float SightRadius, float LoseSightRadius, float PeripheralVisionAngle) const
{
	// 시야(사거리) 설정
	if (SightConfig)
	{
		// 인지 거리
		SightConfig->SightRadius = SightRadius;
		// 인지 해제 거리
		SightConfig->LoseSightRadius = LoseSightRadius;
		// 시야 각도
		SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle;

		AIPerception->ConfigureSense(*SightConfig);
	}
}
