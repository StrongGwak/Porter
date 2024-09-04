// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PHeroBulletStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPHeroBulletStruct :public FTableRowBase
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator MeshRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HitBoxSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsThrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpinRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpinPivot;

};
