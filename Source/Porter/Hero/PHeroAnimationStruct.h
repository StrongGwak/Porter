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
	UAnimSequence* WalkAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* RunAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* HitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* DieAim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackAnim;
};