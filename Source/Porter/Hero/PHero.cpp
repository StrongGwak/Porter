// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHero.h"

#include "PHeroAnimInstance.h"
#include "PHeroWeaponStruct.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Hero/PHeroStruct.h"
#include "Hero/PHeroAIController.h"
#include "Hero/PHeroBulletPoolManager.h"
#include "Hero/PHeroBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APHero::APHero()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Bullet이 Player Type을 무시하기 때문에 Hero도 Object Type을 Player로 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Hero"));
	GetMesh()->SetCollisionProfileName(TEXT("Hero"));
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
	GunPosition->SetRelativeLocation(FVector3d(-10, 0, 30.0f));
	
	// 투사체 생성 위치
	RangeAttackPosition = CreateDefaultSubobject<USceneComponent>(TEXT("RangeAttackPosition"));
	RangeAttackPosition->SetupAttachment(GunPosition);
	RangeAttackPosition->SetWorldLocation(GunPosition->GetComponentLocation());
	RangeAttackPosition->SetRelativeLocation(FVector3d(50, 0, 0.0f));
	
	// AI Controller 할당
	AIControllerClass = APHeroAIController::StaticClass();
	// 월드에 배치되거나 스폰될 때 AI Controller에 의해 제어되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 투사체 매니저 클래스 할당
	BulletPoolManagerClass = APHeroBulletPoolManager::StaticClass();

	// 영웅 콜리전 제거 및 중력 제거
	SetActorEnableCollision(true);
	GetCharacterMovement()->GravityScale=0;

	// 무기 데이터 테이블 불러오기
	WeaponDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Porter/Develop/Hero/DT_HeroWeapon.DT_HeroWeapon"));
	
	// 무기 스켈레탈 메시 추가
	MainWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	MainWeaponMesh->SetupAttachment(GetMesh());
	MainWeaponMesh->SetCollisionProfileName(TEXT("Hero"));
	
	// 무기 박스 콜리전 생성
	MainWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	MainWeaponCollision->SetCollisionProfileName(TEXT("Hero"));
	MainWeaponCollision->SetupAttachment(MainWeaponMesh);
	MainWeaponCollision->SetBoxExtent(FVector(0,0,0));

	// 무기 스켈레탈 메시 추가
	SubWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SubWeaponSkeletalMesh"));
	SubWeaponMesh->SetupAttachment(GetMesh());
	SubWeaponMesh->SetCollisionProfileName(TEXT("Hero"));
	
	// 무기 박스 콜리전 생성
	SubWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SubWeaponCollision"));
	SubWeaponCollision->SetCollisionProfileName(TEXT("Hero"));
	SubWeaponCollision->SetupAttachment(SubWeaponMesh);
	SubWeaponCollision->SetBoxExtent(FVector(0,0,0));	
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

	if(bIsKorean)
	{
		TwoHandAttachRotation();
	}
	//DrawDebugPoint(GetWorld(), GunPosition->GetComponentLocation(), 10, FColor(52, 220, 239), true);
	//DrawDebugPoint(GetWorld(), RangeAttackPosition->GetComponentLocation(), 10, FColor(52, 0, 0), true);
	if (AttackTarget)
	{
		//DrawDebugLine(GetWorld(), GunPosition->GetComponentLocation(), AttackTarget->GetActorLocation(), FColor::Emerald, false, 5, 0, 5);
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
			BulletPoolManager->SetImpactSound(ImpactSound);
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
		HeroAniminstance->OnDrawNotifyDelegate.AddDynamic(this, &APHero::Draw);
		HeroAniminstance->OnDieNotifyDelegate.AddDynamic(this, &APHero::Detach);
		GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &APHero::OnAttackEnded);
	}
	
	FPHeroWeaponStruct* WeaponStructptr = FindWeapon(Name);
	if (WeaponStructptr != nullptr)
	{
		MainWeaponMesh->SetSkeletalMesh(WeaponStructptr->MainMesh);
		MainWeaponMesh->SetRelativeLocation(WeaponStructptr->MainMeshLocation);
		MainWeaponMesh->SetRelativeRotation(WeaponStructptr->MainMeshRotation);
		MainWeaponCollision->SetBoxExtent(WeaponStructptr->MainHitBoxSize);
		MainWeaponCollision->SetRelativeLocation(WeaponStructptr->MainHitBoxLocation);
		MainWeaponCollision->SetRelativeRotation(WeaponStructptr->MainHitBoxRotation);
		MainWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APHero::OnHitBoxOverlap);
		
		SubWeaponMesh->SetSkeletalMesh(WeaponStructptr->SubMesh);
		SubWeaponMesh->SetRelativeLocation(WeaponStructptr->SubMeshLocation);
		SubWeaponMesh->SetRelativeRotation(WeaponStructptr->SubMeshRotation);
		SubWeaponCollision->SetBoxExtent(WeaponStructptr->SubHitBoxSize);
		SubWeaponCollision->SetRelativeLocation(WeaponStructptr->SubHitBoxLocation);
		SubWeaponCollision->SetRelativeRotation(WeaponStructptr->SubHitBoxRotation);
		SubWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APHero::OnHitBoxOverlap);
		
		if (!bIsMelee)
		{
			SubWeaponMesh->SetHiddenInGame(true);
		}
		
		if (WeaponStructptr->bIsAttachSocket)
		{
			MainWeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponStructptr->MainSocketName);
			SubWeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponStructptr->SubSocketName);
		}
		
		if (WeaponStructptr->bIsSetLeaderMesh)
		{
			MainWeaponMesh->SetLeaderPoseComponent(GetMesh());
			SubWeaponMesh->SetLeaderPoseComponent(GetMesh());
			MainWeaponCollision->AttachToComponent(MainWeaponMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponStructptr->MainSocketName);
		}
		
		if(bIsKorean)
		{
			FVector HandSocketLocation = GetMesh()->GetSocketLocation(FName("LeftHandSocket"));
			FVector WeaponLocation = MainWeaponMesh->GetComponentLocation();
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WeaponLocation, HandSocketLocation);
			FRotator AdjustedRotation = LookAtRotation + FRotator(90.0f, 0.0f, 0.0f); // X축에서 90도 회전하여 밑면이 바라보도록 설정
			MainWeaponMesh->SetWorldRotation(AdjustedRotation);
		}
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
	Stat.bIsKorean = bIsKorean;
	Stat.AttackSound = AttackSound;
	Stat.DrawSound = DrawSound;
	Stat.DieSound = DieSound;
	Stat.HitSound = HitSound;
	Stat.ImpactSound = ImpactSound;
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
	bIsKorean = UpdateStats.bIsKorean;
	AttackSound = UpdateStats.AttackSound;
	DrawSound = UpdateStats.DrawSound;
	DieSound = UpdateStats.DieSound;
	HitSound = UpdateStats.HitSound;
	ImpactSound = UpdateStats.ImpactSound;
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
	// 처음 적을 발견시 공격 애니메이션 시작
	if (HeroAniminstance && !bIsDead)
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
			}
		}
	}
}

void APHero::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	static bool bCanHit = true;
	if (OtherActor && OtherActor != this && bCanHit)
	{
		if (OtherActor->ActorHasTag("Enemy"))
		{
			bCanHit = false;
			// 블루프린트 AnyDamage 이벤트 호출
			FDamageEvent DamageEvent;   // Generic damage event
			OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this);
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, OtherActor->GetActorLocation(), 0.5);
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(
				TimerHandle, 
				[this]()
				{
					bCanHit = true;  // 쿨타임이 끝나면 다시 히트 가능
				}, 
				1.0f,  // 쿨타임 5초
				false  // 반복하지 않음
			);
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
			FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GunPosition->GetComponentLocation(), AttackTarget->GetActorLocation());
			// 투사체 회전 설정 투사체의 방향이 위를 바라보기 때문에 Pitch - 90
			Bullet->SetActorRotation(FRotator(LookAtRotator.Pitch - 90, LookAtRotator.Yaw, LookAtRotator.Roll));
			// 투사체 발사 위치로 설정
			Bullet->SetActorLocation(RangeAttackPosition->GetComponentLocation());
			// 투사체에 힘 적용
			Bullet->Fire(GunPosition->GetForwardVector());
		}
	}
}

void APHero::StartAttack()
{
	if (AttackTarget)
	{
		bIsLookingForward = false;
		bIsLookingTarget = true;

		UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation(), 0.5);
		
		//원거리 공격
		if (!bIsMelee)
		{
			RangeAttack();
			SubWeaponMesh->SetHiddenInGame(true);
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
		FRotator TargetRotation = FRotator(0, LookAtRotation.Yaw, LookAtRotation.Roll - 10);
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도
		
		// 새로운 회전 각도를 설정
		GunPosition->SetWorldRotation(LookAtRotation);
		if (bIsMelee)
		{
			HeroAniminstance->SetRotator(FRotator(0, GunPosition->GetRelativeRotation().Yaw,GunPosition->GetRelativeRotation().Roll + 30));
		}
		else
		{
			HeroAniminstance->SetRotator(FRotator(0, GunPosition->GetRelativeRotation().Yaw,GunPosition->GetRelativeRotation().Roll));
		}
		
		
		if (NewRotation.Equals(LookAtRotation, 0.1f))
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
	FRotator NewAnimRotation = FMath::RInterpTo(HeroAniminstance->GetRotator(), FRotator(0.0f, 0.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도

	// 새로운 회전 각도를 설정
	GunPosition->SetWorldRotation(NewRotation);
	HeroAniminstance->SetRotator(NewAnimRotation);	
	if (NewRotation.Equals(TargetRotation, 0.1f))
	{
		bIsLookingForward = false;
	}
}

void APHero::TwoHandAttachRotation()
{
	FVector HandSocketLocation = GetMesh()->GetSocketLocation(FName("LeftHandSocket"));
	FVector WeaponLocation = MainWeaponMesh->GetComponentLocation();
	FRotator CurrentRotation = MainWeaponMesh->GetComponentRotation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WeaponLocation, HandSocketLocation);
	FRotator AdjustedRotation = LookAtRotation + FRotator(90.0f, 0.0f, 0.0f); // X축에서 90도 회전하여 밑면이 바라보도록 설정
	//FRotator NewRotation = FMath::RInterpTo(CurrentRotation, AdjustedRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f는 회전 속도

	MainWeaponMesh->SetWorldRotation(AdjustedRotation);
}

void APHero::GetDamage(int TakenDamage)
{
	HP -= TakenDamage;
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(), 0.5);
	if (HP < 1)
	{
		Die();
	}
}


void APHero::Die()
{
	bIsDead = true;
	if (HeroAniminstance)
	{
		HeroAniminstance->StopAttack();
		HeroAniminstance->Die();
	}
	UGameplayStatics::PlaySoundAtLocation(this, DieSound, GetActorLocation(), 0.5);
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APHero::DestroyHero, 5.0f, false);
}

void APHero::Draw()
{
	SubWeaponMesh->SetHiddenInGame(false);
	UGameplayStatics::PlaySoundAtLocation(this, DrawSound, GetActorLocation(), 0.5);
}

void APHero::Detach()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	// 영웅 콜리전 제거 및 중력 제거
	SetActorEnableCollision(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetLinearDamping(5);
	GetMesh()->SetAngularDamping(5);
	GetMesh()->AddImpulse(GetActorForwardVector() * -10000);
	MainWeaponMesh->SetSimulatePhysics(true);
	MainWeaponMesh->SetLinearDamping(5);
	MainWeaponMesh->SetAngularDamping(5);
	SubWeaponMesh->SetSimulatePhysics(true);
	SubWeaponMesh->SetLinearDamping(5);
	SubWeaponMesh->SetAngularDamping(5);
}

void APHero::DestroyHero()
{
	Destroy();
}

void APHero::SetIndex(int NewIndex)
{
	Index = NewIndex;
}
