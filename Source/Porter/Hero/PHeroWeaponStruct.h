// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PHeroWeaponStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPHeroWeaponStruct :public FTableRowBase
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HitBoxSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttachSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLeftHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTwoHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MainSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SubSocketName;
};