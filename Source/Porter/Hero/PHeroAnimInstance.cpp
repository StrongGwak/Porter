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
		SittingAnim = AnimationStructptr->SittingAnim;
		IdleAnim = AnimationStructptr->IdleAnim;
		HitAnim = AnimationStructptr->HitAnim;
		DieAnim = AnimationStructptr->DieAim;
		AttackAnim = AnimationStructptr->AttackAnim;
	}
}

void UPHeroAnimInstance::SetRotator(FRotator NewRotator)
{
	RotationToTarget = NewRotator;
}

FRotator UPHeroAnimInstance::GetRotator() const
{
	return RotationToTarget;
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

void UPHeroAnimInstance::Attack(float AttackSpeed)
{
	Montage_Play(AttackAnim, AttackSpeed);
}

void UPHeroAnimInstance::StopAttack()
{
	if (Montage_IsPlaying(AttackAnim))
	{
		Montage_Stop(0.25f, AttackAnim);
	}
}

void UPHeroAnimInstance::Hit()
{
	Montage_Play(HitAnim);
}

void UPHeroAnimInstance::Die()
{
	Montage_Play(DieAnim);
}

void UPHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	

}

void UPHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UPHeroAnimInstance::AnimNotify_Fire() const
{
	// 공격 노티파이 델리게이트 호출
	OnAttackNotifyDelegate.Broadcast();
}

void UPHeroAnimInstance::AnimNotify_Hit() const
{
}

void UPHeroAnimInstance::AnimNotify_Die() const
{
	OnDieNotifyDelegate.Broadcast();
}

void UPHeroAnimInstance::AnimNotify_Draw() const
{
	OnDrawNotifyDelegate.Broadcast();
}
