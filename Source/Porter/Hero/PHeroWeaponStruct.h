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
	USkeletalMesh* MainMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SubMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MainMeshLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator MainMeshRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MainHitBoxSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MainHitBoxLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator MainHitBoxRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SubMeshLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SubMeshRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SubHitBoxSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SubHitBoxLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SubHitBoxRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttachSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSetLeaderMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MainSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SubSocketName;
};