// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Hero/PHero.h"
#include "PHeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTER_API UPHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPHeroAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override; //애니메이션이 생성되면 호출되는 함수

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //프레임마다 호출 함수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	APHero* Hero;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	FRotator RotationToTarget;	

	
};
