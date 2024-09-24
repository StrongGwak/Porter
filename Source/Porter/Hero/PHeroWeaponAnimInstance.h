// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PHeroWeaponStruct.h"
#include "PHeroWeaponAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTER_API UPHeroWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPHeroWeaponAnimInstance();

	void StartAttack();

	void StopAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Idle;
	
};
