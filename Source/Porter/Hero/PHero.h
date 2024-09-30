// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/PHeroBulletPoolManager.h"
#include "Hero/PHeroStruct.h"
#include "Hero/PHeroWeaponStruct.h"
#include "Hero/PHeroAnimInstance.h"
#include "Hero/PHeroWeaponAnimInstance.h"
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
	
	FPHeroWeaponStruct* FindWeapon(FName RowName) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
// Hero Stat
protected:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FName Name;
	
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FName KorName;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int HP;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int Damage;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	FVector MeshLocation;
	
	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* BodyMesh;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* HairMesh;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* TopMesh;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* BottomMesh;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* ShoesMesh;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* AccessorieMesh;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BodyMeshComponent;
	
	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HairMeshComponent;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* TopMeshComponent;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BottomMeshComponent;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ShoesMeshComponent;

	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* AccessorieMeshComponent;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float SightRadius;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float VisionAngle;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UStaticMesh* BulletMesh;
	
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float BulletSpeed;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	bool bIsMelee;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 Index;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	bool bIsKorean;

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

	UPROPERTY(EditAnywhere)
	UPHeroAnimInstance* HeroAniminstance;

	UFUNCTION()
	void OnAttackEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditDefaultsOnly)
	UDataTable* WeaponDataTable;
	
	UPROPERTY(EditAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MainWeaponMesh;

	UPROPERTY(EditAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"));
	UBoxComponent* MainWeaponCollision;

	UPROPERTY(EditAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SubWeaponMesh;

	UPROPERTY(EditAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"));
	UBoxComponent* SubWeaponCollision;

	UPROPERTY(EditAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"));
	bool bIsTwoHand;

	UPROPERTY(EditAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"));
	FName SubSocketName;

	UPROPERTY(EditAnywhere, Category = Sound, Meta = (AllowPrivateAccess = "true"));
	USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, Category = Sound, Meta = (AllowPrivateAccess = "true"));
	USoundBase* DieSound;
	
	UPROPERTY(EditAnywhere, Category = Sound, Meta = (AllowPrivateAccess = "true"));
	USoundBase* DrawSound;

	UPROPERTY(EditAnywhere, Category = Sound, Meta = (AllowPrivateAccess = "true"));
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Sound, Meta = (AllowPrivateAccess = "true"));
	USoundBase* ImpactSound;
	
	UFUNCTION()
	void Detach();

	UFUNCTION()
	void DestroyHero();

	UFUNCTION()
	void Draw();

public:
	void Initialize(FPHeroStruct HeroStruct);
	
	FPHeroStruct GetHeroStats() const;
	
	void SetHeroStats(const FPHeroStruct& UpdateStats);
	
	void FindTarget(AActor* Target);

	UFUNCTION()
	void StartAttack();
	
	void StopAttack();
	
	void LookTarget();
	
	void LookForward();

	void TwoHandAttachRotation();
	
	void RangeAttack() const;
	
	UFUNCTION(BlueprintCallable, Category="Function")
	void GetDamage(int TakenDamage);

	void Die();
	
	void SetIndex(int NewIndex);

	UFUNCTION()
	void OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);

	bool bIsDead;
	
};