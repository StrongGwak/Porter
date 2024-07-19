// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHero.h"
#include "Hero/PHeroStruct.h"
#include "Hero/PHeroAIController.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APHero::APHero()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("/Game/Porter/Develop/Hero/ABP_PHeroAnimation.ABP_PHeroAnimation_C"));
	if (AnimInstance.Succeeded()) 
	{
		GetMesh()->SetAnimInstanceClass(AnimInstance.Class);
	}
}


APHero::APHero(const FPHeroStruct& HeroInfo)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈 메시 할당
	if (HeroInfo.SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(HeroInfo.SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector3d(0.0f, 0.0f, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("/Game/Porter/Develop/Hero/ABP_PHeroAnimation.ABP_PHeroAnimation_C"));
	if (AnimInstance.Succeeded()) 
	{
		GetMesh()->SetAnimInstanceClass(AnimInstance.Class);
	}

	// 매개변수로 온 구조체의 정보를 영웅 멤버변수에 할당
	Damage = HeroInfo.Damage;
	AttackSpeed = HeroInfo.AttackSpeed;
	AttackAnim = HeroInfo.AttackAnim;
	SightRadius = HeroInfo.SightRadius;
	VisionAngle = HeroInfo.VisionAngle;
	
	// AI Controller 할당
	AIControllerClass = APHeroAIController::StaticClass();
	// 월드에 배치되거나 스폰될 때 AI Controller에 의해 제어되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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

	// 스켈레탈 메시 할당
	if (TestStruct.SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(TestStruct.SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector3d(0.0f, 0.0f, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	// 매개변수로 온 구조체의 정보를 영웅 멤버변수에 할당
	Damage = TestStruct.Damage;
	AttackSpeed = TestStruct.AttackSpeed;
	AttackAnim = TestStruct.AttackAnim;
	SightRadius = TestStruct.SightRadius;
	VisionAngle = TestStruct.VisionAngle;
	AttackAnim = TestStruct.AttackAnim;

	// AI Controller 할당
	AIControllerClass = APHeroAIController::StaticClass();
	// 월드에 배치되거나 스폰될 때 AI Controller에 의해 제어되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// AI Controller 캐스팅
	AIController = Cast<APHeroAIController>(GetController());
	if (AIController)
	{
		// AI Controller의 시야 정보 설정 (적 인식 거리)
		AIController->SetSightConfig(SightRadius, SightRadius + 100.0f, VisionAngle);
	}
	
}

// Called every frame
void APHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APHero::FindTarget(AActor* Target)
{
	AttackTarget = Target;
	if (!GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &APHero::StartAttack, AttackSpeed, true);
	}
}

void APHero::StartAttack()
{
	if (AttackTarget)
	{
		UE_LOG(LogTemp, Log, TEXT("Attack: %s"), *AttackTarget->GetName());

		/*FVector TargetLocation = AttackTarget->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();
		
		// 대상 액터를 향한 회전 각도를 계산
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

		// 현재 회전을 천천히 목표 회전으로 보간
		FRotator CurrentRotation = GetMesh()->GetComponentRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도

		// 새로운 회전 각도를 설정
		GetMesh()->SetWorldRotation(NewRotation);
		PlayAnimMontage(AttackAnim);*/
		FVector TargetLocation = AttackTarget->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();
		
		// 대상 액터를 향한 회전 각도를 계산
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

		// 현재 회전을 천천히 목표 회전으로 보간
		FRotator CurrentRotation = GetMesh()->GetComponentRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도

		// 새로운 회전 각도를 설정
		SetActor(NewRotation);
		PlayAnimMontage(AttackAnim);
	}
	
}

void APHero::StopAttack()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
	{
		AttackTarget = nullptr;
		UE_LOG(LogTemp, Log, TEXT("StopAttack"));
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		// 현재 회전을 천천히 목표 회전으로 보간
		FRotator CurrentRotation = GetMesh()->GetRelativeRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, FRotator(0.0f, -90.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도

		// 새로운 회전 각도를 설정
		GetMesh()->SetWorldRotation(NewRotation);
	}
}


