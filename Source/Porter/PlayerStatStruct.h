// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStatStruct.generated.h"

/**
 * 
 */
class PORTER_API PlayerStatStruct
{
public:
	PlayerStatStruct();
	~PlayerStatStruct();
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerStatsStruct
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerWalkSpeed = 600;	// 평상시 이동속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerMaxStamina = 100;	// 달릴 때 자원 최댓값		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerStamina	= 100;		// 달릴 때 자원 현재값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerRunSpeed = 1000;	// 달릴 때 이동속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerHeroNumber = 15;	// 최대 히어로 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerMaxHP = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerHP = 5;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeroAttackSpeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeroMeleeDamage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeroCasterDamage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeroCriticalChance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeroCriticalDamage = 0;
};
