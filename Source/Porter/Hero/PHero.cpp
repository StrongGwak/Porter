// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHero.h"

#include "Components/CapsuleComponent.h"
#include "Hero/PHeroStruct.h"
#include "Hero/PHeroAIController.h"
#include "Hero/PHeroBulletPoolManager.h"
#include "Hero/PHeroBullet.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APHero::APHero()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Bullet이 Player Type을 무시하기 때문에 Hero도 Object Type을 Player로 설정
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel2);
	GetCapsuleComponent()->SetCapsuleHalfHeight(60);

	// 애니메이션 인스턴스 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> TempAnimInstance(TEXT("/Game/Porter/Develop/Hero/ABP_PHeroAnimation.ABP_PHeroAnimation_C"));
	if (TempAnimInstance.Succeeded()) 
	{
		GetMesh()->SetAnimInstanceClass(TempAnimInstance.Class);
	}

	GunPosition = CreateDefaultSubobject<USceneComponent>(TEXT("GunPosition"));
	GunPosition->SetupAttachment(GetCapsuleComponent());

	// 투사체 생성 위치
	RangeAttackPosition = CreateDefaultSubobject<USceneComponent>(TEXT("RangeAttackPosition"));
	RangeAttackPosition->SetupAttachment(GunPosition);
	RangeAttackPosition->SetRelativeLocation(FVector3d(50, 0, 50.0f));

	// AI Controller 할당
	AIControllerClass = APHeroAIController::StaticClass();
	// 월드에 배치되거나 스폰될 때 AI Controller에 의해 제어되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// AI Controller 캐스팅
	AIController = Cast<APHeroAIController>(GetController());

	BulletPoolManagerClass = APHeroBulletPoolManager::StaticClass();
	
}

// Called when the game starts or when spawned
void APHero::BeginPlay()
{
	Super::BeginPlay();

	if (BulletPoolManagerClass) {
		BulletPoolManager = GetWorld()->SpawnActor<APHeroBulletPoolManager>(BulletPoolManagerClass);
	}
	
	// Bullet이 Player Type을 무시하기 때문에 Hero도 Object Type을 Player로 설정
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel2);
	// 스켈레탈 메시 할당
	if (TestStruct.SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(TestStruct.SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector3d(-20.0f, 0.0f, -120.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel2);
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

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &APHero::OnMontageEnded);
	}
	
}

// Called every frame
void APHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 정면을 바라보는 함수
	if (bIsLookingForward)
	{
		LookForward();
	}

	// 타겟을 바라보는 함수
	if (bIsLookingTarget)
	{
		LookTarget();
	}
}


void APHero::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackAnim)
	{
		if (bInterrupted)
		{
			bIsLookingTarget = false;
			bIsLookingForward = true;
		}
		else
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(AttackAnim);
			}
		}
	}
}

void APHero::Initialize(FPHeroStruct HeroStruct)
{
	// 스켈레탈 메시 할당
	if (TestStruct.SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(HeroStruct.SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector3d(0.0f, 0.0f, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	// 매개변수로 온 구조체의 정보를 영웅 멤버변수에 할당
	Damage = HeroStruct.Damage;
	AttackSpeed = HeroStruct.AttackSpeed;
	AttackAnim = HeroStruct.AttackAnim;
	SightRadius = HeroStruct.SightRadius;
	VisionAngle = HeroStruct.VisionAngle;
	AttackAnim = HeroStruct.AttackAnim;
	Index = HeroStruct.Index;
	Type = HeroStruct.Type;

	SetHeroStats(StatInformation);
	
	if (AIController)
	{
		// AI Controller의 시야 정보 설정 (적 인식 거리)
		AIController->SetSightConfig(SightRadius, SightRadius + 100.0f, VisionAngle);
	}
}

FPHeroStruct APHero::GetHeroStats() const
{
	return StatInformation;
}

void APHero::SetHeroStats(const FPHeroStruct& UpdateStats)
{
	Damage = UpdateStats.Damage;
	AttackSpeed = UpdateStats.AttackSpeed;
	AttackAnim = UpdateStats.AttackAnim;
	SightRadius = UpdateStats.SightRadius;
	VisionAngle = UpdateStats.VisionAngle;
	AttackAnim = UpdateStats.AttackAnim;
	Index = UpdateStats.Index;
	Type = UpdateStats.Type;
	
	StatInformation = UpdateStats;
}

void APHero::FindTarget(AActor* Target)
{
	// 타겟 할당
	AttackTarget = Target;
	
	if (AttackTarget)
	{
		bIsLookingForward = false;
		bIsLookingTarget = true;
	}
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(AttackAnim);
	}
	
}

void APHero::StartAttack()
{
	if (AttackTarget)
	{
		bIsLookingForward = false;
		bIsLookingTarget = true;
	}
	RangeAttack();
}

void APHero::RangeAttack() const
{
	
	if (BulletPoolManager)
	{
		APHeroBullet* Bullet = BulletPoolManager->GetBullet();
		if (Bullet)
		{
			FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(RangeAttackPosition->GetComponentLocation(), AttackTarget->GetActorLocation());
			Bullet->SetActorRotation(FRotator(LookAtRotator.Pitch - 90, LookAtRotator.Yaw, LookAtRotator.Roll));
			Bullet->SetActorLocation(RangeAttackPosition->GetComponentLocation());
			Bullet->Initialize(TestStruct.BulletMesh, TestStruct.BulletSpeed, Damage, RangeAttackPosition->GetForwardVector());
			DrawDebugBox(GetWorld(), RangeAttackPosition->GetComponentLocation(), FVector(10, 10, 10), FColor::Purple, true, -1, 0, 1);
			UE_LOG(LogTemp, Log, TEXT("Position %f, %f ,%f"), AttackTarget->GetActorLocation().X, AttackTarget->GetActorLocation().Y, AttackTarget->GetActorLocation().Z);
		}
	}
}

void APHero::StopAttack() const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (AnimInstance->Montage_IsPlaying(AttackAnim))
		{
			AnimInstance->Montage_Stop(0.25f, AttackAnim);
		}
	}
}

void APHero::LookTarget()
{
	if(AttackTarget)
	{
		FRotator CurrentRotation = GunPosition->GetComponentRotation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GunPosition->GetComponentLocation(), AttackTarget->GetActorLocation());
		FRotator TargetRotation = FRotator(LookAtRotation.Pitch, LookAtRotation.Yaw, 0);
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 20.0f); // 5.0f는 회전 속도
		AnimRotation = NewRotation + FRotator(0, 90, 0);

		// 새로운 회전 각도를 설정
		GunPosition->SetWorldRotation(NewRotation);
		
		if (NewRotation.Equals(TargetRotation, 0.1f))
		{
			bIsLookingTarget = false;
		}
		else
		{
			bIsLookingTarget = true;
		}
	}
}

void APHero::LookForward()
{
	// 현재 회전을 천천히 목표 회전으로 보간
	FRotator CurrentRotation = GunPosition->GetComponentRotation();
	FRotator TargetRotation = FRotator(0, 0, 0);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도
	AnimRotation = NewRotation + FRotator(0, 90, 0);
	// 새로운 회전 각도를 설정
	GunPosition->SetWorldRotation(NewRotation);

	if (NewRotation.Equals(TargetRotation, 0.1f))
	{
		bIsLookingForward = false;
	}
}


