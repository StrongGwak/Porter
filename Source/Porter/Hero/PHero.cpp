// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHero.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	BulletPoolManagerClass = APHeroBulletPoolManager::StaticClass();

	SetActorEnableCollision(false);
	GetCharacterMovement()->GravityScale=0;

	AnimRotation = GetMesh()->GetComponentRotation();
}

// Called when the game starts or when spawned
void APHero::BeginPlay()
{
	Super::BeginPlay();

	if (BulletPoolManagerClass) {
		BulletPoolManager = GetWorld()->SpawnActor<APHeroBulletPoolManager>(BulletPoolManagerClass);
		BulletPoolManager->Initialize(TestStruct.BulletMesh, TestStruct.BulletSpeed, TestStruct.Damage);
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &APHero::OnMontageEnded);
	}

	// ========== Test Code =========
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
	APHeroAIController* AIController = Cast<APHeroAIController>(GetController());
	if (AIController)
	{
		// AI Controller의 시야 정보 설정 (적 인식 거리)
		AIController->SetSightConfig(SightRadius, SightRadius + 100.0f, VisionAngle);
	}
	// ========== Test Code =========
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

// 애니메이션 종료시 실행하는 함수
void APHero::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackAnim)
	{
		// 애니메이션이 끝나기전에 종료됐다면
		if (bInterrupted)
		{
			bIsLookingTarget = false;
			bIsLookingForward = true;
		}
		else // 애니메이션이 성공적으로 끝났다면
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				// 애니메이션 다시 실행
				AnimInstance->Montage_Play(AttackAnim);
			}
		}
	}
}

void APHero::Initialize(FPHeroStruct HeroStruct)
{
	// 스켈레탈 메시 할당
	if (HeroStruct.SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(HeroStruct.SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector3d(0.0f, 0.0f, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	// Hero Stat 설정
	SetHeroStats(HeroStruct);

	if (BulletPoolManagerClass) {
		BulletPoolManager = GetWorld()->SpawnActor<APHeroBulletPoolManager>(BulletPoolManagerClass);
		BulletPoolManager->Initialize(BulletMesh, BulletSpeed, Damage);
	}
	
	// AI Controller 캐스팅
	APHeroAIController* AIController = Cast<APHeroAIController>(GetController());
	if (AIController)
	{
		// AI Controller의 시야 정보 설정 (적 인식 거리)
		AIController->SetSightConfig(SightRadius, SightRadius + 100.0f, VisionAngle);
	}
}

FPHeroStruct APHero::GetHeroStats() const
{
	FPHeroStruct Stat;
	Stat.HP = HP;
	Stat.Damage = Damage;
	Stat.AttackSpeed = AttackSpeed;
	Stat.SkeletalMesh = SkeletalMesh;
	Stat.AttackAnim = AttackAnim;
	Stat.SightRadius = SightRadius;
	Stat.VisionAngle = VisionAngle;
	Stat.BulletMesh = BulletMesh;
	Stat.BulletSpeed = BulletSpeed;
	Stat.Index = Index;
	Stat.Type = Type;
	return Stat;
}

void APHero::SetHeroStats(const FPHeroStruct& UpdateStats)
{
	HP = UpdateStats.HP;
	Damage = UpdateStats.Damage;
	AttackSpeed = UpdateStats.AttackSpeed;
	AttackAnim = UpdateStats.AttackAnim;
	SightRadius = UpdateStats.SightRadius;
	VisionAngle = UpdateStats.VisionAngle;
	BulletSpeed = UpdateStats.BulletSpeed;
	Index = UpdateStats.Index;
	Type = UpdateStats.Type;
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
	// 처음 적을 발견시 공격 애니메이션 시작
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(AttackAnim);
	}
	
}

void APHero::RangeAttack() const
{
	if (BulletPoolManager)
	{
		// Bullet Pool Manager에서 유효한 Bullet을 가져온다.
		APHeroBullet* Bullet = BulletPoolManager->GetBullet();
		if (Bullet)
		{
			// 투사체 발사 위치에서 타겟을 바라보는 방향 계산
			FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(RangeAttackPosition->GetComponentLocation(), AttackTarget->GetActorLocation());
			// 투사체 회전 설정 투사체의 방향이 위를 바라보기 때문에 Pitch - 90
			Bullet->SetActorRotation(FRotator(LookAtRotator.Pitch - 90, LookAtRotator.Yaw, LookAtRotator.Roll));
			// 투사체 발사 위치로 설정
			Bullet->SetActorLocation(RangeAttackPosition->GetComponentLocation());
			// 투사체에 힘 적용
			Bullet->Fire(RangeAttackPosition->GetForwardVector());
		}
	}
}

void APHero::GetDamage(int TakenDamage)
{
	HP -= TakenDamage;
	if (HP < 1)
	{
		Die();
	}
}

void APHero::Die()
{
	UE_LOG(LogTemp, Log, TEXT("%s Hero Die"), *GetName());
}

void APHero::SetIndex(int NewIndex)
{
	Index = NewIndex;
}

void APHero::StartAttack()
{
	if (AttackTarget)
	{
		bIsLookingForward = false;
		bIsLookingTarget = true;
		// 조건문으로 근거리 원거리 공격
		RangeAttack();
	}
	
	
}

void APHero::StopAttack()
{
	AttackTarget = nullptr;
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
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도

		// 새로운 회전 각도를 설정
		GunPosition->SetWorldRotation(NewRotation);
		AnimRotation = GunPosition->GetComponentRotation() - GetMesh()->GetComponentRotation();
		if (NewRotation.Equals(TargetRotation, 0.1f))
		{
			bIsLookingTarget = false;
		}
	} else
	{
		bIsLookingTarget = false;
	}
}

void APHero::LookForward()
{
	// 현재 회전을 천천히 목표 회전으로 보간
	FRotator CurrentRotation = GunPosition->GetComponentRotation();
	FRotator TargetRotation = FRotator(0, 0, 0);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도
	AnimRotation = NewRotation;
	// 새로운 회전 각도를 설정
	GunPosition->SetWorldRotation(NewRotation);

	if (NewRotation.Equals(TargetRotation, 0.5f))
	{
		bIsLookingForward = false;
	}
}