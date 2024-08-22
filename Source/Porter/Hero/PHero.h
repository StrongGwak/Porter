// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/PHeroBulletPoolManager.h"
#include "Hero/PHeroStruct.h"
#include "GameFramework/Character.h"
#include "PHero.generated.h"

UCLASS()
class PORTER_API APHero : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APHero();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
// Hero Stat
protected:	
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FName Name;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int HP;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int Damage;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int AttackSpeed;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float SightRadius;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float VisionAngle;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UStaticMesh* BulletMesh;
	
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float BulletSpeed;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	bool IsMelee;
	
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 Index;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 Type;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	AActor* AttackTarget;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"));
	USceneComponent* GunPosition;
	
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* RangeAttackPosition;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	bool bIsLookingTarget;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	bool bIsLookingForward;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APHeroBulletPoolManager> BulletPoolManagerClass;

	UPROPERTY(EditAnywhere)
	APHeroBulletPoolManager* BulletPoolManager;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	UPROPERTY(EditAnywhere)
	FRotator AnimRotation;

	void Initialize(FPHeroStruct HeroStruct);
	
	FPHeroStruct GetHeroStats() const;
	void SetHeroStats(const FPHeroStruct& UpdateStats);
	void FindTarget(AActor* Target);
	void StartAttack();
	void StopAttack();
	void LookTarget();
	void LookForward();
	void RangeAttack() const;
	
	UFUNCTION(BlueprintCallable, Category="Function")
	void GetDamage(int TakenDamage);
	
	void Die();
	void SetIndex(int NewIndex);

};