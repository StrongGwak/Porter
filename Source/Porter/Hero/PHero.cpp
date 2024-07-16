// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHero.h"
#include "Hero/PHeroStruct.h"
#include "Hero/PHeroAIController.h"

// Sets default values
APHero::APHero()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


APHero::APHero(const FPHeroStruct& HeroInfo)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (HeroInfo.SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(HeroInfo.SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector3d(0.0f, 0.0f, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	// 매개변수로 온 구조체의 정보를 영웅 멤버변수에 할당
	Damage = HeroInfo.Damage;
	AttackSpeed = HeroInfo.AttackSpeed;
	AttackAnim = HeroInfo.AttackAnim;
	SightRadius = HeroInfo.SightRadius;
	VisionAngle = HeroInfo.VisionAngle;
	
	// AI Controller 할당
	AIControllerClass = APHeroAIController::StaticClass();
	// AI Controller 캐스팅
	AIController = Cast<APHeroAIController>(GetController());
    if (AIController)
	{
    	// AI Controller의 시야 정보 설정 (적 인식 거리)
		AIController->SetSightConfig(SightRadius, SightRadius + 10.0f, VisionAngle);
	}
	
	
}

// Called when the game starts or when spawned
void APHero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


