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
	FVector MeshLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* HairMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* TopMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* BottomMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* ShoesMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* AccessorieMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VisionAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMelee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimBlueprint> AnimationBlueprint;
};
