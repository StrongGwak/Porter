// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PHeroBulletStruct.h"
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

	void Initialize(FName RowName, int Damage);

	FPHeroBulletStruct* FindBullet(FName RowName) const;

	void SetImpactSound(USoundBase* ImpactSound);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APHeroBullet> BulletClass;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* BulletDataTable;

	UPROPERTY(EditAnywhere)
	int32 PoolSize = 15;

	TArray<APHeroBullet*> BulletPool;

	USoundBase* ImpactSound;
	
};