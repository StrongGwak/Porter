// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroAnimInstance.h"

UPHeroAnimInstance::UPHeroAnimInstance()
{
	
}

void UPHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Hero = Cast<APHero>(GetOwningActor());
}

void UPHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Mesh만 회전
	if (Hero)
	{
		RotationToTarget = Hero->AnimRotation;
	}
	
}

void UPHeroAnimInstance::AnimNotify_Fire()
{
	// 애니메이션에 추가돼있는 Fire Notify 실행 시 공격 시작
	if (Hero)
	{
		Hero->StartAttack();
	}
}
