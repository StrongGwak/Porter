// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroAnimInstance.h"
#include "PHeroAnimationStruct.h"

UPHeroAnimInstance::UPHeroAnimInstance()
{
	AnimationDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Porter/Develop/Hero/DT_HeroAnimation.DT_HeroAnimation"));
}

void UPHeroAnimInstance::SetAnimation(FName RowName)
{
	FPHeroAnimationStruct* AnimationStructptr = FindAnimation(RowName);

	if (AnimationStructptr != nullptr)
	{
		IdleAnim = AnimationStructptr->IdleAnim;
		WalkAnim = AnimationStructptr->WalkAnim;
		RunAnim = AnimationStructptr->RunAnim;
		HitAnim = AnimationStructptr->HitAnim;
		DieAim = AnimationStructptr->DieAim;
		AttackAnim = AnimationStructptr->AttackAnim;
	}
}

void UPHeroAnimInstance::SetRotator(FRotator NewRotator)
{
	RotationToTarget = NewRotator;
}

FPHeroAnimationStruct* UPHeroAnimInstance::FindAnimation(FName RowName) const
{
	static const FString ContextString(TEXT("Animation Null"));
	if (AnimationDataTable)
	{
		FPHeroAnimationStruct* AnimationStructptr = AnimationDataTable->FindRow<FPHeroAnimationStruct>(RowName, ContextString);
		if (AnimationStructptr)
		{
			return AnimationStructptr;
		}
	}
	return nullptr;
}

void UPHeroAnimInstance::Attack()
{
	Montage_Play(AttackAnim);
}

void UPHeroAnimInstance::StopAttack()
{
	if (Montage_IsPlaying(AttackAnim))
	{
		Montage_Stop(0.25f, AttackAnim);
	}
}

void UPHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UPHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UPHeroAnimInstance::AnimNotify_Fire()
{
	// 공격 노티파이 델리게이트 호출
	OnAttackNotifyDelegate.Broadcast();
}
