// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PHeroAnimationStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPHeroAnimationStruct :public FTableRowBase
{
public:
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* SittingAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* IdleAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DieAim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackAnim;
};