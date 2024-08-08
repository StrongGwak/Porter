// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroBulletPoolManager.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APHeroBulletPoolManager::APHeroBulletPoolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 투사체 클래스 설정
	BulletClass = APHeroBullet::StaticClass();
}

// Called when the game starts or when spawned
void APHeroBulletPoolManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APHeroBulletPoolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

APHeroBullet* APHeroBulletPoolManager::GetBullet()
{
	for (APHeroBullet* Bullet : BulletPool)
	{
		// 수정할 곳
		if (!Bullet->IsActorActive())
		{
			Bullet->SetActorEnableCollision(true);
			Bullet->SetActorHiddenInGame(false);
			return Bullet;
		}
	}
	// 유효한 Bullet이 없을때 생성
	APHeroBullet* NewBullet = GetWorld()->SpawnActor<APHeroBullet>(BulletClass);
	BulletPool.Add(NewBullet);
	return NewBullet;
}

void APHeroBulletPoolManager::ReturnBullet(APHeroBullet* Bullet)
{
	// 사용하기 전 상태로 비활성화
	Bullet->SetActorEnableCollision(false);
	Bullet->SetActorHiddenInGame(true);
	Bullet->SetActorLocation(FVector::ZeroVector);
	Bullet->ProjectileMovementComponent->bSimulationEnabled = false;
}

void APHeroBulletPoolManager::Initialize(UStaticMesh* NewStaticMesh, float NewSpeed, float NewDamage)
{
	StaticMesh = NewStaticMesh;
	Speed = NewSpeed;
	Damage = NewDamage;
	
	if (BulletClass)
	{
		// Bullet 생성
		for (int i = 0; i < PoolSize; i++)
		{
			APHeroBullet* Bullet = GetWorld()->SpawnActor<APHeroBullet>(BulletClass);
			Bullet->SetActorEnableCollision(false);
			Bullet->SetActorHiddenInGame(true);
			Bullet->SetBulletPoolManager(this);
			Bullet->Initialize(StaticMesh, Speed, Damage);
			BulletPool.Add(Bullet);
		}
	}
}
