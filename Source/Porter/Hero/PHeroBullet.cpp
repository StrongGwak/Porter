// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroBullet.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Hero/PHero.h"

// Sets default values
APHeroBullet::APHeroBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BulletBoxCollision->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	BulletBoxCollision->OnComponentHit.AddDynamic(this, &APHeroBullet::OnBoxHit);
	RootComponent = BulletBoxCollision;

	UArrowComponent* Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	RootComponent->SetupAttachment(Arrow);
	Arrow->SetRelativeRotation(FRotator(0, 90.0f, 0));

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
	Destroy();
}

void APHeroBullet::Initilize(UStaticMeshComponent* NewStaticMesh, float NewSpeed, float NewDamage)
{
	StaticMesh = NewStaticMesh;
	RootComponent->SetupAttachment(StaticMesh);
	Speed = NewSpeed;
	Damage = NewDamage;

	ProjectileMovementComponent->InitialSpeed = Speed;
	
}
