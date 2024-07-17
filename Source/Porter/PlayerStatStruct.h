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
	float PlayerWalkSpeed = 400;	// 평상시 이동속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerRunSpeed = 1000;	// 달릴 때 이동속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerMaxStamina = 100;	// 스태미나 최댓값		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerDecreaseStamina = 33;	// 달릴 때 스태미나 소모값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerIncreaseStamina = 20;	// 0까지 가지 않았을 때 스태미나 충전값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerZeroToHundredIncreaseSteamin = 10;	// 0까지 가고 나서 스태미나 충전값
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHeroNum = 5;	// 최대 히어로 수, 절대로 15를 넘기지 말 것
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlusMaxHeroHP = 0;	// 증감값. 1넣으면 용병 체력이 5+1=6이 됨

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
