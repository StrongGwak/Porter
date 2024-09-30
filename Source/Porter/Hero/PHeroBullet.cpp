// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroBullet.h"

#include "PHeroBulletPoolManager.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APHeroBullet::APHeroBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 박스 콜리전 생성
	BulletBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	// 콜리전 설정
	BulletBoxCollision->BodyInstance.SetCollisionProfileName("Bullet");
	// 플레이어는 무시
	BulletBoxCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	// 온박스히트 함수 추가
	BulletBoxCollision->OnComponentHit.AddDynamic(this, &APHeroBullet::OnBoxHit);
	// 박스 콜리전 추가
	RootComponent = BulletBoxCollision;

	// 스태틱 메시 초기화
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	// 메시의 콜리전 비활성화
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetCollisionProfileName("NoCollision");
	// 메시 추가
	StaticMesh->SetupAttachment(RootComponent);

	// 투사체 컴포넌트 초기화
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	// 투사체 컴포넌트 움직임 비활성화
	ProjectileMovementComponent->bSimulationEnabled = false;
	// 회전 비활성화
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	// 튕김 비활성화
	ProjectileMovementComponent->bShouldBounce = false;
	// 컴포넌트 추가
	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());

	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating"));
}

// Called when the game starts or when spawned
void APHeroBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APHeroBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APHeroBullet::OnBoxHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// Enemy인지 태그로 식별
	if (Hit.GetActor()->ActorHasTag("Enemy"))
	{
		// 블루프린트 AnyDamage 이벤트 호출
		FDamageEvent DamageEvent;   // Generic damage event
		Hit.GetActor()->TakeDamage(Damage, DamageEvent, nullptr, this);
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.GetActor()->GetActorLocation(), 0.5);
		
		// 맞은 컴포넌트 유효성 검사
		UPrimitiveComponent* TargetComponent = Hit.GetComponent();
		if (TargetComponent && TargetComponent->IsA<USceneComponent>())
		{
			// Scene컴포넌트로 캐스팅
			USceneComponent* SceneComponent  = Cast<USceneComponent>(TargetComponent);
			// 자식 컴포넌트 배열 생성
			TArray<USceneComponent*> ChildComponents;
			// 자식 컴포넌트 배열에 할당
			SceneComponent->GetChildrenComponents(true, ChildComponents);
			for (USceneComponent* Child : ChildComponents)
			{
				if (Child->ComponentHasTag("HitProjectile"))
				{
					// 컴포넌트 식별
					if (Child->bHiddenInGame)
					{
						// 자식 컴포넌트를 StaticMesh 컴포넌트로 캐스팅
						UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Child);

						// 해당 액터에 투사체 흔적 남기기
						// 자식 컴포넌트 숨기기 비활성화
						StaticMeshComponent->SetHiddenInGame(false);
						// 투사체의 메시를 복사
						StaticMeshComponent->SetStaticMesh(StaticMesh->GetStaticMesh());
						// 맞은 위치로 흔적 위치 설정
						StaticMeshComponent->SetWorldLocation(HitComp->GetComponentLocation());
						// 투사체 각도로 흔적 각도 설정
						StaticMeshComponent->SetWorldRotation(StaticMesh->GetComponentRotation());
						// 타이머로 시간 경과 시 흔적 비활성화
						FTimerHandle HitTimerHandle;
						FTimerDelegate Callback = FTimerDelegate::CreateLambda([StaticMeshComponent]() { StaticMeshComponent->SetHiddenInGame(true); });
						GetWorld()->GetTimerManager().SetTimer(HitTimerHandle, Callback, 10.0f, false);
						break;
					}
				}
			}
		}
	}
	// 투사체 비활성화
	BulletPoolManager->ReturnBullet(this);
}

void APHeroBullet::Initialize(FPHeroBulletStruct* Struct, float NewDamage)
{
	// 스태틱 메시 할당
	StaticMesh->SetStaticMesh(Struct->Mesh);
	// 위치 설정
	StaticMesh->SetRelativeLocation(Struct->MeshLocation);
	// 회전 설정
	StaticMesh->SetRelativeRotation(Struct->MeshRotator);
	// 박스 콜리전 크기 조절
	BulletBoxCollision->SetBoxExtent(Struct->HitBoxSize);// 박스 콜리전 크기 조절
	// 투사체의 속도와 데미지 설정
	Speed = Struct->Speed;
	Damage = NewDamage;

	// 투사체 속도 설정
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = 8000;

	if (Struct->IsThrow)
	{
		RotatingComponent->RotationRate = Struct->SpinRotation;
		RotatingComponent->PivotTranslation = Struct->SpinPivot;
	}

}

bool APHeroBullet::IsActorActive() const
{
	// 투사체가 비활성화 됐는지 확인
	return !IsHidden() && GetActorEnableCollision();
}

void APHeroBullet::Fire(FVector FirePosition) const
{
	// 투사체 움직임 활성화
	ProjectileMovementComponent->bSimulationEnabled = true;
	// 투사체의 움직일 컴포넌트 설정 (충돌시 None으로 설정됨)
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	// 투사체에 힘 적용
	ProjectileMovementComponent->Velocity = FirePosition * ProjectileMovementComponent->InitialSpeed;
}

void APHeroBullet::SetBulletPoolManager(APHeroBulletPoolManager* Manager)
{
	// 매니저 설정
	BulletPoolManager = Manager;
}

void APHeroBullet::UpdateBullet(int UpdateDamage, int UpdateSpeed)
{
	Damage = UpdateDamage;
	Speed = UpdateSpeed;
}

void APHeroBullet::SetImpactSound(USoundBase* NewImpactSound)
{
	ImpactSound = NewImpactSound;
}
