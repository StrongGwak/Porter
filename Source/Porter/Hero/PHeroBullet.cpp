// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroBullet.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APHeroBullet::APHeroBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BulletBoxCollision->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	BulletBoxCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	BulletBoxCollision->OnComponentHit.AddDynamic(this, &APHeroBullet::OnBoxHit);
	BulletBoxCollision->SetBoxExtent(FVector3d(4.0f, 4.0f, 4.0f));
	RootComponent = BulletBoxCollision;
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeRotation(FRotator(90.0f, 0, 0));
	Arrow->SetRelativeLocation(FVector3d(0, 0, 20.0f));
	Arrow->SetArrowLength(40);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionProfileName("NoCollision");
	StaticMesh->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->bShouldBounce = false;

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
	
	StaticMesh->SetStaticMesh(NewStaticMesh);
	StaticMesh->SetRelativeLocation(FVector3d(0, 0, 10.0f));
	Speed = NewSpeed;
	Damage = NewDamage;

	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = 3000;
	
	ProjectileMovementComponent->Velocity = FirePosition * Speed;
	UE_LOG(LogTemp, Log, TEXT("Velocity : %f, %f, %f"), ProjectileMovementComponent->Velocity.X, ProjectileMovementComponent->Velocity.Y, ProjectileMovementComponent->Velocity.Z);

}
