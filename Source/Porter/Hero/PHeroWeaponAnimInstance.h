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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAttack;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimMontage* Attack;
	
};
