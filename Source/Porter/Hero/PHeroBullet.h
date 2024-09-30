// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PHeroBulletStruct.h"
#include "PHeroBullet.generated.h"

// 전방 선언
class APHeroBulletPoolManager;

UCLASS()
class PORTER_API APHeroBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APHeroBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBoxHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Initialize(FPHeroBulletStruct* Struct, float NewDamage);

	bool IsActorActive() const;

	void Fire(FVector FirePosition) const;

	// 불렛 매니저에 대한 참조 추가
	void SetBulletPoolManager(APHeroBulletPoolManager* Manager);

	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementComponent;

	void UpdateBullet(int UpdateDamage, int UpdateSpeed);

	void SetImpactSound(USoundBase* NewImpactSound);

protected:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float Damage;

	UPROPERTY(VisibleAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BulletBoxCollision;

	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	APHeroBulletPoolManager* BulletPoolManager;

	UPROPERTY(EditAnywhere, Category = Rotating, Meta = (AllowPrivateAccess = "true"))
	class URotatingMovementComponent* RotatingComponent;

	UPROPERTY(EditAnywhere, Category = Sound, Meta = (AllowPrivateAccess = "true"))
	USoundBase* ImpactSound;
};
