// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroAIController.h"
#include "Hero/PHero.h"
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
	Hero = Cast<APHero>(GetPawn());

}

void APHeroAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (OldTarget && OldTarget->IsPendingKillPending())
	{
		AIPerception->ForgetActor(OldTarget);
		OldTarget = nullptr;
		// 인지 하고있는 액터를 할당하기 위한 액터 배열
		TArray<AActor*> Knowns;
		// 현재 인지하고 있는 액터들을 액터 배열에 할당
		AIPerception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), Knowns);
		OnPerceptionUpdated(Knowns);
	} else if (OldTarget == nullptr)
	{
		// 대상이 없다면 공격 중지
		Hero->StopAttack();
	}
}

void APHeroAIController::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{
	// 인지 하고있는 액터를 할당하기 위한 액터 배열
	TArray<AActor*> Knowns;
	// 현재 인지하고 있는 액터들을 액터 배열에 할당
	AIPerception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), Knowns);
	
	if (!Knowns.IsEmpty())
	{
		AActor* Target = nullptr;
		// 최소거리는 최대 시야로 시작
		float minDistance = SightConfig->SightRadius;
		// 인지하고 있는 액터 반복문
		for (AActor* Actor : Knowns)
		{
			// Enemy인 액터만 탐색
			if (!Actor->ActorHasTag("Enemy"))
			{
				continue;
			}
			// 액터와의 거리 계산
			float Distance = CalculateDistanceToActor(Actor);
			// 가장 가까운 액터 탐색
			if (Distance < minDistance)
			{
				minDistance = Distance;
				Target = Actor;
			}
		}
		if (Target != OldTarget)
		{
			// 영웅에게 타겟 전달
			Hero->FindTarget(Target);
			OldTarget = Target;
		}
	}
	else
	{
		// 대상이 없다면 공격 중지
		Hero->StopAttack();
		OldTarget = nullptr;
	}
}

float APHeroAIController::CalculateDistanceToActor(AActor* OtherActor) const
{
	// 영웅의 위치
	FVector MyLocation = GetPawn()->GetActorLocation();
	// 해당 액터의 위치
	FVector OtherActorLocation = OtherActor->GetActorLocation();
	// 사이의 거리 계산
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
		// 시야 감지 설정
		AIPerception->ConfigureSense(*SightConfig);
	}
}