// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PHeroBullet.generated.h"

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

	void Initialize(UStaticMesh* NewStaticMesh, float NewSpeed, float NewDamage, FVector FirePosition);

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
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* Arrow;
	
};
