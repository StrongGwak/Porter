// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PHeroStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPHeroStruct :public FTableRowBase
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VisionAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMelee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Type = -1;
};
