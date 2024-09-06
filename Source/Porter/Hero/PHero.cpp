// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHero.h"

#include "PHeroAnimInstance.h"
#include "PHeroWeaponStruct.h"
#include "Components/BoxComponent.h"
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

	// 스켈레탈메시 생성
	HairMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMeshComponent->SetupAttachment(GetMesh());
	TopMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TopMesh"));
	TopMeshComponent->SetupAttachment(GetMesh());
	BottomMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BottomMesh"));
	BottomMeshComponent->SetupAttachment(GetMesh());
	ShoesMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoesMesh"));
	ShoesMeshComponent->SetupAttachment(GetMesh());
	AccessorieMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AccessorieMesh"));
	AccessorieMeshComponent->SetupAttachment(GetMesh());

	GunPosition = CreateDefaultSubobject<USceneComponent>(TEXT("GunPosition"));
	GunPosition->SetupAttachment(GetCapsuleComponent());
	
	// 투사체 생성 위치
	RangeAttackPosition = CreateDefaultSubobject<USceneComponent>(TEXT("RangeAttackPosition"));
	RangeAttackPosition->SetupAttachment(GunPosition);
	RangeAttackPosition->SetRelativeLocation(FVector3d(60, 0, 35.0f));
	
	// AI Controller 할당
	AIControllerClass = APHeroAIController::StaticClass();
	// 월드에 배치되거나 스폰될 때 AI Controller에 의해 제어되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 투사체 매니저 클래스 할당
	BulletPoolManagerClass = APHeroBulletPoolManager::StaticClass();

	// 영웅 콜리전 제거 및 중력 제거
	SetActorEnableCollision(false);
	GetCharacterMovement()->GravityScale=0;

	// 무기 데이터 테이블 불러오기
	WeaponDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Porter/Develop/Hero/DT_HeroWeapon.DT_HeroWeapon"));
	
	// 무기 스켈레탈 메시 추가
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponMesh->SetupAttachment(GetMesh());
	
	// 무기 박스 콜리전 생성
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetBoxExtent(FVector(0,0,0));

	// 애니메이션 인스턴스 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> TempWeaponAnimInstance(TEXT("/Game/Porter/Develop/Hero/ABP_PHeroWeaponAnimation.ABP_PHeroWeaponAnimation_C"));
	if (TempWeaponAnimInstance.Succeeded()) 
	{
		WeaponMesh->SetAnimInstanceClass(TempWeaponAnimInstance.Class);
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

void APHero::Initialize(FPHeroStruct HeroStruct)
{
	// 스켈레탈 메시 할당
	if (HeroStruct.BodyMesh)
	{
		GetMesh()->SetSkeletalMesh(HeroStruct.BodyMesh);
		GetMesh()->SetRelativeLocation(HeroStruct.MeshLocation);
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

		if(HeroStruct.HairMesh)
		{
			HairMeshComponent->SetSkeletalMesh(HeroStruct.HairMesh);
			HairMeshComponent->SetLeaderPoseComponent(GetMesh());
		}
		if(HeroStruct.TopMesh)
		{
			TopMeshComponent->SetSkeletalMesh(HeroStruct.TopMesh);
			TopMeshComponent->SetLeaderPoseComponent(GetMesh());
		}
		if(HeroStruct.BottomMesh)
		{
			BottomMeshComponent->SetSkeletalMesh(HeroStruct.BottomMesh);
			BottomMeshComponent->SetLeaderPoseComponent(GetMesh());
		}
		if(HeroStruct.ShoesMesh)
		{
			ShoesMeshComponent->SetSkeletalMesh(HeroStruct.ShoesMesh);
			ShoesMeshComponent->SetLeaderPoseComponent(GetMesh());
		}
		if(HeroStruct.AccessorieMesh)
		{
			AccessorieMeshComponent->SetSkeletalMesh(HeroStruct.AccessorieMesh);
			AccessorieMeshComponent->SetLeaderPoseComponent(GetMesh());
		}
	}

	// Hero Stat 설정
	SetHeroStats(HeroStruct);

	if (!bIsMelee)
	{
		if (BulletPoolManagerClass) {
			BulletPoolManager = GetWorld()->SpawnActor<APHeroBulletPoolManager>(BulletPoolManagerClass);
			BulletPoolManager->Initialize(Name, Damage);
		}
	}
	
	// AI Controller 캐스팅
	APHeroAIController* AIController = Cast<APHeroAIController>(GetController());
	if (AIController)
	{
		// AI Controller의 시야 정보 설정 (적 인식 거리)
		AIController->SetSightConfig(SightRadius, SightRadius + 100.0f, VisionAngle);
	}

	if (AnimInstance)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstance);
		HeroAniminstance = Cast<UPHeroAnimInstance>(GetMesh()->GetAnimInstance());
		HeroAniminstance->SetAnimation(Name);
		HeroAniminstance->OnAttackNotifyDelegate.AddDynamic(this, &APHero::StartAttack);
		GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &APHero::OnAttackEnded);
	}
	
	FPHeroWeaponStruct* WeaponStructptr = FindWeapon(Name);
	if (WeaponStructptr != nullptr)
	{
		WeaponMesh->SetSkeletalMesh(WeaponStructptr->SkeletalMesh);
		WeaponCollision->SetBoxExtent(WeaponStructptr->HitBoxSize);
		WeaponCollision->SetRelativeLocation(WeaponStructptr->MeshLocation);
		if (WeaponStructptr->bIsAttachSocket)
		{
			WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponStructptr->MainSocketName);
		}
		
	}

	if (UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance())
	{
		WeaponAniminstance = Cast<UPHeroWeaponAnimInstance>(WeaponAnimInstance);
		//AnimInstance->OnMontageEnded.AddDynamic(this, &APHero::OnAttackEnded);
	}
}

FPHeroStruct APHero::GetHeroStats() const
{
	FPHeroStruct Stat;
	Stat.Name = Name;
	Stat.KorName = KorName;
	Stat.HP = HP;
	Stat.Damage = Damage;
	Stat.AttackSpeed = AttackSpeed;
	Stat.MeshLocation = MeshLocation;
	Stat.AnimInstance = AnimInstance;
	Stat.BodyMesh = BodyMesh;
	Stat.HairMesh = HairMesh;
	Stat.TopMesh = TopMesh;
	Stat.BottomMesh = BottomMesh;
	Stat.ShoesMesh = ShoesMesh;
	Stat.AccessorieMesh = AccessorieMesh;
	Stat.SightRadius = SightRadius;
	Stat.VisionAngle = VisionAngle;
	Stat.bIsMelee = bIsMelee;
	Stat.Index = Index;
	return Stat;
}

void APHero::SetHeroStats(const FPHeroStruct& UpdateStats)
{
	Name = UpdateStats.Name;
	KorName = UpdateStats.KorName;
	HP = UpdateStats.HP;
	Damage = UpdateStats.Damage;
	AttackSpeed = UpdateStats.AttackSpeed;
	MeshLocation = UpdateStats.MeshLocation;
	AnimInstance = UpdateStats.AnimInstance;
	BodyMesh = UpdateStats.BodyMesh;
	HairMesh = UpdateStats.HairMesh;
	TopMesh = UpdateStats.TopMesh;
	BottomMesh = UpdateStats.BottomMesh;
	ShoesMesh = UpdateStats.ShoesMesh;
	AccessorieMesh = UpdateStats.AccessorieMesh;
	SightRadius = UpdateStats.SightRadius;
	VisionAngle = UpdateStats.VisionAngle;
	bIsMelee = UpdateStats.bIsMelee;
	Index = UpdateStats.Index;
}

FPHeroWeaponStruct* APHero::FindWeapon(FName RowName) const
{
	static const FString ContextString(TEXT("Weapon Null"));
	if (WeaponDataTable)
	{
		FPHeroWeaponStruct* WeaponStructptr = WeaponDataTable->FindRow<FPHeroWeaponStruct>(RowName, ContextString);
		if (WeaponStructptr)
		{
			return WeaponStructptr;
		}
	}
	return nullptr;
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
	
	if (bIsMelee && WeaponAniminstance)
	{
		WeaponAniminstance->StartAttack();
	}
	// 처음 적을 발견시 공격 애니메이션 시작
	if (HeroAniminstance)
	{
		HeroAniminstance->Attack(AttackSpeed);
	}
}

// 애니메이션 종료시 실행하는 함수
void APHero::OnAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage)
	{
		// 애니메이션이 끝나기전에 종료됐다면
		if (bInterrupted)
		{
			bIsLookingTarget = false;
		}
		else // 애니메이션이 성공적으로 끝났다면
		{
			if (HeroAniminstance)
			{
				HeroAniminstance->Attack(AttackSpeed);
				//무기 애니메이션 인스턴스 적용해서 공격애니메이션해야함
				if (bIsMelee)
				{
					WeaponAniminstance->StartAttack();
				}
				
			}
		}
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

void APHero::StartAttack()
{
	if (AttackTarget)
	{
		bIsLookingForward = false;
		bIsLookingTarget = true;

		//원거리 공격
		if (!bIsMelee)
		{
			RangeAttack();
		}		
	}
}

void APHero::StopAttack()
{
	AttackTarget = nullptr;
	if (!AttackTarget)
	{
		bIsLookingTarget = false;
		bIsLookingForward = true;
	}
	if (bIsMelee && WeaponAniminstance)
	{
		WeaponAniminstance->StopAttack();
	}
	if (HeroAniminstance)
	{
		HeroAniminstance->StopAttack();
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
		HeroAniminstance->SetRotator(GunPosition->GetRelativeRotation());
		WeaponMesh->SetRelativeRotation(FRotator(0, GunPosition->GetRelativeRotation().Yaw, 0));
		if (NewRotation.Equals(TargetRotation, 0.1f))
		{
			bIsLookingTarget = false;
		}
	}
}

void APHero::LookForward()
{
	// 현재 회전을 천천히 목표 회전으로 보간
	FRotator TargetRotation = GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(GunPosition->GetComponentRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도
	FRotator NewAnimRotation = FMath::RInterpTo(AnimRotation, FRotator(0.0f, 0.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도

	// 새로운 회전 각도를 설정
	GunPosition->SetWorldRotation(NewRotation);
	HeroAniminstance->SetRotator(NewAnimRotation);
	WeaponMesh->SetRelativeRotation(NewAnimRotation);
	
	if (NewRotation.Equals(TargetRotation, 0.1f))
	{
		bIsLookingForward = false;
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
