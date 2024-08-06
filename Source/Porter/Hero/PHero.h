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
	FString HName;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int Damage;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int AttackSpeed;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float SightRadius;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float VisionAngle;
	
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 Index;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPHeroStruct TestStruct;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	AActor* AttackTarget;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"));
	class USceneComponent* GunPosition;
	
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RangeAttackPosition;

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
	void FindTarget(AActor* Target);
	void StartAttack();
	void StopAttack() const;
	void LookTarget();
	void LookForward();
	void RangeAttack() const;

private:
	FTimerHandle AttackTimerHandle;

	FTimerHandle LookForwardTimerHandle;

	FTimerHandle LookTargetTimerHandle;
};