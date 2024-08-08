// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/PHeroBullet.h"
#include "GameFramework/Actor.h"
#include "PHeroBulletPoolManager.generated.h"

UCLASS()
class PORTER_API APHeroBulletPoolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APHeroBulletPoolManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APHeroBullet* GetBullet();
	
	void ReturnBullet(APHeroBullet* Bullet);

	void Initialize(UStaticMesh* NewStaticMesh, float NewSpeed, float NewDamage);

protected:
	UPROPERTY(EditAnywhere, Category = Bullet, Meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(EditAnywhere, Category = Bullet, Meta = (AllowPrivateAccess = "true"))
	float Damage;

	UPROPERTY(EditAnywhere, Category = Bullet, Meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APHeroBullet> BulletClass;

	UPROPERTY(EditAnywhere)
	int32 PoolSize = 15;

	TArray<APHeroBullet*> BulletPool;
};