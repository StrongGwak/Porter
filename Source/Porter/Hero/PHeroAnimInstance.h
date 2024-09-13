// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PHeroAnimationStruct.h"
#include "PHeroAnimInstance.generated.h"

/**
 * 
 */
// 공격 노티파이 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackNotifyDelegate);

UCLASS()
class PORTER_API UPHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPHeroAnimInstance();

	void SetAnimation(FName RowName);

	void SetRotator(FRotator NewRotator);

	void SetSubHandTransform(FTransform NewTransform);

	FPHeroAnimationStruct* FindAnimation(FName RowName) const;

	void Attack(float AttackSpeed);

	void StopAttack();

	// 공격 노티파이 델리게이트
	UPROPERTY()
	FOnAttackNotifyDelegate OnAttackNotifyDelegate;

protected:
	virtual void NativeInitializeAnimation() override; //애니메이션이 생성되면 호출되는 함수

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //프레임마다 호출 함수

	UPROPERTY(EditDefaultsOnly)
	UDataTable* AnimationDataTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero")
	FRotator RotationToTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero")
	UAnimSequence* SittingAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero")
	UAnimSequence* IdleAnim;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero")
	UAnimSequence* HitAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero")
	UAnimSequence* DieAim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero")
	UAnimMontage* AttackAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero")
	FTransform SubHand;

private:
	UFUNCTION()
	void AnimNotify_Fire();

	
	
};
