// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHero.h"

#include "Components/CapsuleComponent.h"
#include "Hero/PHeroStruct.h"
#include "Hero/PHeroAIController.h"
#include "Hero/PHeroBullet.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APHero::APHero()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Bullet이 Player Type을 무시하기 때문에 Hero도 Object Type을 Player로 설정
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel2);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("/Game/Porter/Develop/Hero/ABP_PHeroAnimation.ABP_PHeroAnimation_C"));
	if (AnimInstance.Succeeded()) 
	{
		GetMesh()->SetAnimInstanceClass(AnimInstance.Class);
	}

	RangeAttackPosition = CreateDefaultSubobject<USceneComponent>(TEXT("RangeAttackPosition"));
	RangeAttackPosition->SetupAttachment(GetMesh());
	RangeAttackPosition->SetRelativeLocation(FVector3d(0, 50.0f, 120.0f));
	
}

// Called when the game starts or when spawned
void APHero::BeginPlay()
{
	Super::BeginPlay();

	// Bullet이 Player Type을 무시하기 때문에 Hero도 Object Type을 Player로 설정
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel2);
	// 스켈레탈 메시 할당
	if (TestStruct.SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(TestStruct.SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector3d(0.0f, 0.0f, -90.0f));
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
	
}

// Called every frame
void APHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLookingForward)
	{
		LookForward();
	}
	if (bIsLookingTarget)
	{
		LookTarget();
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

		bIsLookingTarget = true;
		float AnimTime = PlayAnimMontage(AttackAnim);
		RangeAttack();
		/*FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &APHero::OnAttackMontageEnded);*/
		//FTimerHandle AnimTimerHandle;
		//GetWorld()->GetTimerManager().SetTimer(AnimTimerHandle, this, &APHero::RangeAttack, AnimTime / 2, false);
		//GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, AttackAnim);
	}
}

void APHero::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackAnim)
	{
		// 애니메이션이 끝났을 때 불렛 생성
		RangeAttack();

		// 애니메이션이 끝난 후 다시 공격 함수 호출
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &APHero::StartAttack, AttackSpeed, false);
	}
}

void APHero::RangeAttack()
{
	if (AttackTarget)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(RangeAttackPosition->GetComponentLocation(), AttackTarget->GetActorLocation());
		APHeroBullet* Bullet = GetWorld()->SpawnActor<APHeroBullet>(APHeroBullet::StaticClass(), RangeAttackPosition->GetComponentLocation(), FRotator(LookAtRotator.Pitch - 90, LookAtRotator.Yaw, LookAtRotator.Roll), SpawnParams);
		if (Bullet)
		{
			Bullet->Initialize(TestStruct.BulletMesh,TestStruct.BulletSpeed, Damage, RangeAttackPosition->GetRightVector());
		}
	}
}

void APHero::StopAttack()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
	{
		AttackTarget = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		bIsLookingForward = true;
	}
}

void APHero::LookTarget()
{
	if(AttackTarget)
	{
		FRotator CurrentRotation = GetMesh()->GetComponentRotation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetComponentLocation(), AttackTarget->GetActorLocation());
		FRotator TargetRotation = FRotator(0, LookAtRotation.Yaw -90, 0);
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f); // 5.0f는 회전 속도

		// 새로운 회전 각도를 설정
		GetMesh()->SetWorldRotation(NewRotation);

		if (NewRotation.Equals(TargetRotation, 0.1f))
		{
			bIsLookingTarget = false;
		}
	}
	else
	{
		bIsLookingTarget = false;
	}
	
	
}

void APHero::LookForward()
{
	// 현재 회전을 천천히 목표 회전으로 보간
	FRotator CurrentRotation = GetMesh()->GetRelativeRotation();
	FRotator TargetRotation = FRotator(0.0f, -90.0f, 0.0f);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도
	
	// 새로운 회전 각도를 설정
	GetMesh()->SetRelativeRotation(NewRotation);

	if (NewRotation.Equals(TargetRotation, 0.1f))
	{
		bIsLookingForward = false;
	}
}


