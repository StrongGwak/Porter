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

	if (Hero)
	{
		RotationToTarget = Hero->AnimRotation;
	}
	
}

void UPHeroAnimInstance::AnimNotify_Fire()
{
	if (Hero)
	{
		UE_LOG(LogTemp, Log, TEXT("AnimInstance"));
		Hero->StartAttack();
	}
}
