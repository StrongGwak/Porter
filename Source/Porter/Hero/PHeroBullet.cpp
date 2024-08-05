// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroBullet.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APHeroBullet::APHeroBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 박스 콜리전 생성
	BulletBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	// 콜리전 설정
	BulletBoxCollision->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	// 플레이어는 무시
	BulletBoxCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	// 온박스히트 함수 추가
	BulletBoxCollision->OnComponentHit.AddDynamic(this, &APHeroBullet::OnBoxHit);
	// 박스 콜리전 크기 조절
	BulletBoxCollision->SetBoxExtent(FVector3d(4.0f, 4.0f, 4.0f));
	// 박스 콜리전 추가
	RootComponent = BulletBoxCollision;

	// 스태틱 메시 초기화
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	// 메시의 콜리전 비활성화
	StaticMesh->SetCollisionProfileName("NoCollision");
	// 메시 추가
	StaticMesh->SetupAttachment(RootComponent);

	// 투사체 컴포넌트 초기화
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	// 회전 비활성화
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	// 튕김 비활성화
	ProjectileMovementComponent->bShouldBounce = false;
	// 추가
	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	
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
	UE_LOG(LogTemp, Log, TEXT("Hit"));
	
}

void APHeroBullet::Initialize(UStaticMesh* NewStaticMesh, float NewSpeed, float NewDamage, FVector FirePosition)
{
	// 스태틱 메시 할당
	StaticMesh->SetStaticMesh(NewStaticMesh);
	// 위치 설정
	StaticMesh->SetRelativeLocation(FVector3d(0, 0, 10.0f));
	Speed = NewSpeed;
	Damage = NewDamage;

	// 투사체 속도 설정
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = 8000;

	// 투사체에 적용할 힘
	ProjectileMovementComponent->Velocity = FirePosition * ProjectileMovementComponent->InitialSpeed;
}

bool APHeroBullet::IsActorActive() const
{
	return IsActorTickEnabled() && !IsHidden() && !IsActorBeingDestroyed() && GetActorEnableCollision();
}
